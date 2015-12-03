#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#define NIITM_NAME "KBUF"

//struct module *owner;

char *my_buf; 

static ssize_t chardev_read (struct file * name1, char __user * usr, size_t lbuf, loff_t * lt)
{
 int nbytes;
 printk (KERN_INFO "Read KBUF\n");
 nbytes = lbuf-copy_to_user(usr,my_buf, lbuf);
 *lt += nbytes;
 return nbytes;
}

static ssize_t chardev_write (struct file * name2, const char __user * usr, size_t lbuf, loff_t * lt)
{
  int nbytes;
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
 printk (KERN_INFO "Open KBUF\n");
 return 0;
}

static const struct file_operations chardev_fops = {
  .owner = THIS_MODULE,
  .read = chardev_read,
  .write = chardev_write,
  .release = chardev_release, 
  .open = chardev_open,
 };

struct cdev *my_cdev;

static int niitm_init (void)
{
  dev_t first_node; 
  //step 1
  my_buf = kmalloc(PAGE_SIZE, GFP_KERNEL);
  if (my_buf==NULL)
  { 
   printk (KERN_INFO "Create my_buf error\n");     
   return -ENOMEM;
  }
  register_chrdev(855, "KBUF", &chardev_fops);
 
 //step 2
  first_node = MKDEV(855, 55);
  register_chrdev_region(first_node, 1, "MyDevice");
  my_cdev = cdev_alloc();
  cdev_init(my_cdev, &chardev_fops);
  cdev_add(my_cdev, first_node, 1);
  //step 3

  return 0;
	
}


static void niitm_exit (void)
{
 kfree(my_buf);	
}
module_init(niitm_init);
module_exit(niitm_exit);
