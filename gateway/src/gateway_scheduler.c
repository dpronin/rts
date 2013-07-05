/* encoding: UTF-8 */

#include <pthread.h>
#include <stdio.h>

#include "gateway_common.h"
#include "gateway_scheduler.h"
#include "gateway_tasks.h"

task_queue_t g_task_queue[TASK_QUEUE_SIZE];
unsigned int g_head_queue_ind, g_tail_queue_ind;

pthread_mutex_t queue_mutex;

void gateway_scheduler_init_sync(void)
{
    pthread_mutex_init(&queue_mutex, NULL);
}

void gateway_scheduler_deinit_sync(void)
{
    pthread_mutex_destroy(&queue_mutex);
}

void gateway_scheduler_add_task_to_queue(task_data_t task_data)
{
    /* Protected */
    pthread_mutex_lock(&queue_mutex);

        g_task_queue[g_tail_queue_ind].task_data = task_data;
        g_task_queue[g_tail_queue_ind].priority = 0;

        ++g_tail_queue_ind;
        g_tail_queue_ind %= TASK_QUEUE_SIZE;

        if (g_head_queue_ind == g_tail_queue_ind)
        {
            GATEWAY_COMMON_ASSERT(0);
        }

    pthread_mutex_unlock(&queue_mutex);
}

int gateway_scheduler_get_ptask_from_queue(task_data_t *p_task_data)
{
    int result = 1;

    /* Protected */
    pthread_mutex_lock(&queue_mutex);

        if (g_head_queue_ind != g_tail_queue_ind)
        {
            *p_task_data = g_task_queue[g_head_queue_ind].task_data;
            ++g_head_queue_ind;
            g_head_queue_ind %= TASK_QUEUE_SIZE;
        }
        else
        {
            result = 0;
        }

    pthread_mutex_unlock(&queue_mutex);

    return result;
}

void gateway_scheduler_process_task(task_data_t task_data)
{
    switch(task_data.task_type)
    {
        case PUT_MTRX_A_VALUE:
            gateway_tasks_put_mtrx_A_value(task_data.data.mtrx);
            break;
        case PUT_MTRX_C_VALUE:
            gateway_tasks_put_mtrx_C_value(task_data.data.mtrx);
            break;
        case PUT_VECTOR_b_VALUE:
            gateway_tasks_put_vector_b_value(task_data.data.vector);
            break;
        case PUT_VECTOR_d_VALUE:
            gateway_tasks_put_vector_d_value(task_data.data.vector);
            break;
        case MUL_MTRX_A_VALUE_ON_VEC_b_VALUE:
            gateway_tasks_mul_mtrx_A_val_on_vector_b_val(task_data.data.mtrx_col_val_pos);
            break;
        case MUL_MTRX_C_VALUE_ON_VEC_d_VALUE:
            gateway_tasks_mul_mtrx_C_val_on_vector_d_val(task_data.data.mtrx_col_val_pos);
            break;
        case MUL_VECTOR_b_VALUE_ON_MTRX_A_VALUE:
            gateway_tasks_mul_vector_b_val_on_mtrx_A_val(task_data.data.vector_val_pos);
            break;
        case MUL_VECTOR_d_VALUE_ON_MTRX_C_VALUE:
            gateway_tasks_mul_vector_d_val_on_mtrx_C_val(task_data.data.vector_val_pos);
            break;
        case ADD_RES_Ab_VECTOR_VALUE_TO_RES_VECTOR_VALUE:
            gateway_tasks_add_Ab_vector_res_val_to_res_vector_val();
            break;
        case ADD_RES_Cd_VECTOR_VALUE_TO_RES_VECTOR_VALUE:
            gateway_tasks_add_Cd_vector_res_val_to_res_vector_val();
            break;
        case SEND_VELUE_TO_DESTSTATION:
            gateway_tasks_send_value_to_destination_station();
            break;
        default:
            GATEWAY_COMMON_ASSERT(0);
    }
}