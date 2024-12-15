#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/list.h>
#include <linux/ktime.h>
#include <linux/slab.h>

MODULE_AUTHOR("Serhii Popovych <serhii.popovych@globallogic.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static uint parameter = 1;

// 0444 - флаг дозволу на читання параметра
module_param(parameter, uint, 0444);
MODULE_PARM_DESC(parameter, "My description");

struct myStruct {
    struct list_head list;
    ktime_t myTime;
};


static LIST_HEAD(myList);
int counter;

static int __init hello_init(void)
{
    if (parameter > 10) {
   	 pr_err("Error: parameter cannot be greater than 10\n");
   	 return -EINVAL;
    }

    if (parameter == 0 || (parameter <= 10 && parameter >= 5)) {
   	 pr_emerg("Warning: %u\n", parameter);
    }

    counter = 0;
    while (counter != parameter) {
   	 struct myStruct *ptr = kmalloc(sizeof(*ptr), GFP_KERNEL);

   	 ptr->myTime = ktime_get();

   	 list_add_tail(&ptr->list, &myList);
   	 pr_emerg("Hello world!\n");
   	 counter += 1;
    }

    return 0;
}

static void __exit hello_exit(void)
{
    struct myStruct *ptr, *next;

    list_for_each_entry_safe(ptr, next, &myList, list) {
   	 pr_emerg("Time in nanoseconds: %lld\n", ktime_to_ns(ptr->myTime));
   	 list_del(&ptr->list);
   	 kfree(ptr);
    }
}

module_init(hello_init);
module_exit(hello_exit);
