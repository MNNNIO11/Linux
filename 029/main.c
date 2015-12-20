#include "HelloWorld.h"
#include <linux/module.h>
#define NIITM_NAME "niitm hello"
static int niitm_init (void)
{
	print_hello();
        return 0;
}

static void niitm_exit (void)
{
	print_bye();
}
module_init(niitm_init);
module_exit(niitm_exit);
