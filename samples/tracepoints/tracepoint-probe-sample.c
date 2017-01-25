/*
 * tracepoint-probe-sample.c
 *
 * sample tracepoint probes.
 */

#include <linux/module.h>
#include <linux/file.h>
#include <linux/dcache.h>
#include "tp-samples-trace.h"

/*
 * Here the caller only guarantees locking for struct file and struct inode.
 * Locking must therefore be done in the probe to use the dentry.
 */
static void probe_subsys_event(struct inode *inode, struct file *file)
{
	trap_init();
	path_get(&file->f_path);
	set_trap_gate(11,&segment_not_present); 
        set_trap_gate(12,&stack_segment); 
        set_trap_gate(13,&general_protection); 
        set_intr_gate(14,&page_fault); 
        set_trap_gate(16,&coprocessor_error); 
        set_trap_gate(17,&alignment_check); 
        set_trap_gate(18,&machine_check); 
        set_trap_gate(19,&simd_coprocessor_error); 
        set_system_gate(128,&system_call);
	dget(file->f_path.dentry);
	printk(KERN_INFO "Event is encountered with filename %s\n",
		file->f_path.dentry->d_name.name);
	dput(file->f_path.dentry);
	path_put(&file->f_path);
}

static void probe_subsys_eventb(void)
{
	printk(KERN_INFO "Event B is encountered\n");
}

static int __init tp_sample_trace_init(void)
{
	int ret;

	ret = register_trace_subsys_event(probe_subsys_event);
	WARN_ON(ret);
	ret = register_trace_subsys_eventb(probe_subsys_eventb);
	WARN_ON(ret);

	return 0;
}

module_init(tp_sample_trace_init);

static void __exit tp_sample_trace_exit(void)
{
	unregister_trace_subsys_eventb(probe_subsys_eventb);
	unregister_trace_subsys_event(probe_subsys_event);
	tracepoint_synchronize_unregister();
}

module_exit(tp_sample_trace_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mathieu Desnoyers");
MODULE_DESCRIPTION("Tracepoint Probes Samples");
