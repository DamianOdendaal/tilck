/* SPDX-License-Identifier: BSD-2-Clause */

#include <tilck/kernel/sync.h>
#include <tilck/kernel/hal.h>
#include <tilck/kernel/process.h>
#include <tilck/kernel/sched.h>
#include <tilck/kernel/interrupts.h>

bool kmutex_is_curr_task_holding_lock(kmutex *m)
{
   return m->owner_task == get_curr_task();
}

void kmutex_init(kmutex *m, u32 flags)
{
   DEBUG_ONLY(check_not_in_irq_handler());
   bzero(m, sizeof(kmutex));
   m->flags = flags;
   list_init(&m->wait_list);
}

void kmutex_destroy(kmutex *m)
{
   bzero(m, sizeof(kmutex)); // NOTE: !id => invalid kmutex
}

void kmutex_lock(kmutex *m)
{
   disable_preemption();
   DEBUG_ONLY(check_not_in_irq_handler());

   if (!m->owner_task) {

      /* Nobody owns this mutex, just make this task own it */
      m->owner_task = get_curr_task();

      if (m->flags & KMUTEX_FL_RECURSIVE) {
         ASSERT(m->lock_count == 0);
         m->lock_count++;
      }

      enable_preemption();
      return;
   }

   if (m->flags & KMUTEX_FL_RECURSIVE) {

      ASSERT(m->lock_count > 0);

      if (kmutex_is_curr_task_holding_lock(m)) {
         m->lock_count++;
         enable_preemption();
         return;
      }

   } else {
      ASSERT(!kmutex_is_curr_task_holding_lock(m));
   }

   task_set_wait_obj(get_curr_task(), WOBJ_KMUTEX, m, &m->wait_list);
   enable_preemption();
   kernel_yield(); // Go to sleep until someone else is holding the lock.

   /* ------------------- We've been woken up ------------------- */

   /* Now for sure this task should hold the mutex */
   ASSERT(kmutex_is_curr_task_holding_lock(m));

   /*
    * DEBUG check: in case we went to sleep with a recursive mutex, then the
    * lock_count must be just 1 now.
    */
   if (m->flags & KMUTEX_FL_RECURSIVE) {
      ASSERT(m->lock_count == 1);
   }
}

bool kmutex_trylock(kmutex *m)
{
   bool success = false;

   disable_preemption();
   DEBUG_ONLY(check_not_in_irq_handler());

   if (!m->owner_task) {

      /* Nobody owns this mutex, just make this task own it */
      m->owner_task = get_curr_task();
      success = true;

      if (m->flags & KMUTEX_FL_RECURSIVE)
         m->lock_count++;

   } else {

      // There is an owner task

      if (m->flags & KMUTEX_FL_RECURSIVE) {
         if (kmutex_is_curr_task_holding_lock(m)) {
            m->lock_count++;
            success = true;
         }
      }
   }

   enable_preemption();
   return success;
}

void kmutex_unlock(kmutex *m)
{
   disable_preemption();

   DEBUG_ONLY(check_not_in_irq_handler());
   ASSERT(kmutex_is_curr_task_holding_lock(m));

   if (m->flags & KMUTEX_FL_RECURSIVE) {

      ASSERT(m->lock_count > 0);

      if (--m->lock_count > 0) {
         enable_preemption();
         return;
      }

      // m->lock_count == 0: we have to really unlock the mutex
   }

   m->owner_task = NULL;

   /* Unlock one task waiting to acquire the mutex 'm' (if any) */
   if (!list_is_empty(&m->wait_list)) {

      wait_obj *task_wo =
         list_first_obj(&m->wait_list, wait_obj, wait_list_node);

      task_info *ti = CONTAINER_OF(task_wo, task_info, wobj);

      m->owner_task = ti;

      if (m->flags & KMUTEX_FL_RECURSIVE)
         m->lock_count++;

      ASSERT(ti->state == TASK_STATE_SLEEPING);
      task_reset_wait_obj(ti);

   } // if (!list_is_empty(&m->wait_list))

   enable_preemption();
}
