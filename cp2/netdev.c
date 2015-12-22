#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/pid.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <net/arp.h>

#define NIITM_NAME "KBUF"

//struct module *owner;

char *my_buf; 
volatile uint32_t d_irq = 0;

struct dev_stat {
int f_open;
int f_read;
int f_write;
int f_ioctl;
//int f_irq;
};

struct mynet_device
{
  struct net_device *dev;
  /**/
  int xmit;
  int bytes;
  unsigned char netdata[256];
} *mynetdev;
 struct net_device *devnet = NULL;

struct pinfo {
int pid;
char buf[500];
};

struct pinfo p_info;
struct dev_stat info;

irqreturn_t dev_irq (int irq_num, void* count)
{
    WARN_ON(count != &d_irq);

    (*(volatile uint32_t*)count)++;
    return IRQ_NONE;
}

static ssize_t chardev_read (struct file * name1, char __user * usr, size_t lbuf, loff_t * lt)
{
 int nbytes;
 info.f_read++;
 printk (KERN_INFO "Read KBUF\n");
 nbytes = lbuf-copy_to_user(usr,my_buf, lbuf);
 *lt += nbytes;
 return nbytes;
}

static ssize_t chardev_write (struct file * name2, const char __user * usr, size_t lbuf, loff_t * lt)
{
  int nbytes;
  info.f_write++;
  printk (KERN_INFO "Write KBUF\n");
  nbytes = lbuf-copy_from_user(my_buf,usr,lbuf);
  *lt += nbytes;
  return nbytes;
}

int chardev_release (struct inode * i, struct file * name)
{
return 0;
}

int chardev_open (struct inode * i, struct file * name)
{
 info.f_open++;
 printk (KERN_INFO "Open KBUF\n");
 return 0;
}

/*loff_t chardev_llseek(struct file *file, loff_t offset, int whence)
{
  return 0;
}*/

int ret;
long chardev_unlocked_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
  info.f_ioctl++;
  if (cmd == 0x123)
  {
    copy_to_user((void*)arg, &info, sizeof(struct dev_stat));
  }  
  if (cmd == 0x134)
  {
    	 copy_to_user((void __user *)arg, (void *)&d_irq, sizeof(uint32_t));
	//printk(KERN_INFO "DEVSTAT = %d (%d bytes)\n", dev_irq, ret);
	//ret = 0;
  }
  if (cmd == 0x145)
  {
   // struct pid *pid_struct;
    struct task_struct *ptask = current;
   // __get_user(p_info.pid, (int __user *)arg);
   // pid_struct = find_vpid(p_info.pid);
   // ptask = pid_task(pid_struct,PIDTYPE_PID);
    sprintf(p_info.buf,"Current process info: task is '%s', pid = %d, parent is '%s'",ptask->comm, ptask->pid, ptask->parent->comm);
    copy_to_user((void*)arg, (char*)&p_info, sizeof(struct pinfo));
  }
    if (cmd == 0x156)
  {
   struct mynet_device *priv = netdev_priv(devnet);
    copy_to_user((void*)arg, (void *)priv, sizeof(struct mynet_device));
  } 
  return 0;
}

static const struct file_operations chardev_fops = {
  .owner = THIS_MODULE,
  .read = chardev_read,
  .write = chardev_write,
  .release = chardev_release, 
  .open = chardev_open,
  .unlocked_ioctl = chardev_unlocked_ioctl,
 };

int mynetdev_open (struct net_device *dev)
{
 //info.f_open++;
 printk (KERN_INFO "Open netdev\n");
 return 0;
}
int mynetdev_stop (struct net_device *dev)
{
 printk (KERN_INFO "Stop netdev\n");
 return 0;
}

static netdev_tx_t mynetdev_startxmit(struct sk_buff *skb, struct net_device *dev)
{
  int sc;
  struct mynet_device *priv = netdev_priv(dev);
  priv->xmit++;
  priv->bytes += skb->len;
 /* memset(priv->netdata, 0, 255);
  if (skb->len > 256)
  {
    memcpy(priv->netdata, &skb->data[skb->len -256], 256);
  }
  else
     memcpy(priv->netdata, skb->data, skb->len);*/
  for (sc=0;sc<50;sc++)
  {
    priv->netdata[sc] = skb->data[sc];
  }
 // priv->netdata = skb->data;
  printk (KERN_INFO "Transmit ok!\n");
  printk (KERN_INFO "xmit = %d\n", priv->xmit);
  printk (KERN_INFO "bytes = %d\n", priv->bytes);
 // printk (KERN_INFO "skbdata = %s\n", skb->data);
  for (sc=0;sc<50;sc++)
  {
    printk (KERN_INFO "%02x", priv->netdata[sc]&0xff);
   // priv->netdata[sc] = skb->data[sc];
  }
  //printk (KERN_INFO "data = %s\n", priv->netdata);
  //devnet = priv;
 // mynetdev->xmit = priv->xmit;
 // mynetdev->bytes = priv->bytes;
 // memcpy(mynetdev->netdata, priv->netdata, 256);
 // dev_kfree_skb_any(skb);
  return NET_XMIT_DROP;
}
struct net_device_stats* mynetdev_getstat (struct net_device *dev)
{
 //
 return 0;

};
void mynetdev_tx_timeout (struct net_device *dev)
{
 //
}
static const struct net_device_ops mynetdev_ops = {
 .ndo_open = mynetdev_open,
 .ndo_stop = mynetdev_stop,
 .ndo_start_xmit = mynetdev_startxmit,
// .ndo_tx_timeout = mynetdev_tx_timeout,
 //.ndo_get_stats = mynetdev_getstat,
};

struct cdev *my_cdev;
dev_t first_node;

static void mynetdev_init(struct net_device *dev)
{
  struct mynet_device *priv = netdev_priv(dev);
  priv->dev = dev;
  priv->xmit = 0;
  priv->bytes = 0;
 // priv->netdata[0]=0;
  ether_setup(dev);
 // dev->watchdog_timeout = timeout;
  dev->netdev_ops = &mynetdev_ops;
  dev->flags = IFF_NOARP;
}
int rez;
/*static struct net_device *mynetdev_create(const char *name)
{
 // struct net_device *dev;
  devnet = alloc_netdev(sizeof(struct mynet_device), name, NET_NAME_UNKNOWN, mynetdev_init);
  if (my_buf==NULL)
  { 
   printk (KERN_INFO "alloc netdev error\n");     
   //
  }
  mynetdev = netdev_priv(devnet);
  mynetdev->dev = devnet;
  rez = register_netdev(devnet);
  return devnet; 
};*/

static int niitm_init (void)
{
//step 1
  my_buf = kmalloc(PAGE_SIZE, GFP_KERNEL);
  if (my_buf==NULL)
  { 
   printk (KERN_INFO "Create my_buf error\n");     
   return -ENOMEM;
  }

  register_chrdev(850, "kbuf", &chardev_fops);

 //step 2
  first_node = MKDEV(850, 50);
  register_chrdev_region(first_node, 1, "MyDevice");
  my_cdev = cdev_alloc();
  cdev_init(my_cdev, &chardev_fops);
  cdev_add(my_cdev, first_node, 1);
  //step 3
  if(request_irq(19, dev_irq,IRQF_SHARED, "KBUF_IRQ", (void *)&d_irq)) {
	printk(KERN_ERR "request irq failed\n");
	return -EBADF; }
 //step 4
    devnet = alloc_netdev(sizeof(struct mynet_device), "mynetdev", NET_NAME_UNKNOWN, &mynetdev_init);
  if (devnet==NULL)
  { 
   printk (KERN_INFO "alloc netdev error\n");     
   //
  }
 // mynetdev = netdev_priv(devnet);
 // mynetdev->dev = devnet;
  rez = register_netdev(devnet);
 // mynetdev_create("mynetdev");
 // devnet = alloc_netdev(sizeof(struct mynet_device), );
  //mynetdev_init(dev);
  
  return 0;
	
}


static void niitm_exit (void)
{
 synchronize_irq(19);
 free_irq(19, (void *)&d_irq);	
 kfree(my_buf);	
 cdev_del(my_cdev);
 unregister_chrdev_region(first_node, 1);
 unregister_netdev(devnet);
 free_netdev(devnet);
}
module_init(niitm_init);
module_exit(niitm_exit);
MODULE_LICENSE("GPL");
