/* To print "Hello, World!" */
#include <linux/module.h>
void print_hello (void)
{
 printk (KERN_INFO "Hello, World!\n");
}
void print_bye (void)
{
 printk (KERN_INFO "Bye!\n");
}

int niitm_init (void)
{

return 0;
}
void niitm_exit (void)
{

}

EXPORT_SYMBOL(print_hello);
EXPORT_SYMBOL(print_bye);
