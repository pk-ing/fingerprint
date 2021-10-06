/*
 * @Author: Aurora-zk
 * @Date: 2021-03-14 12:39:09
 * @LastEditors: Aurora-zk
 * @LastEditTime: 2021-10-05 20:39:18
 * @contact: pk-ing@nyist.edu.cn
 * @FilePath: \fingerprint\fingerprint_sample.c
 */

#include <fingerprint.h>
#define THREAD_PRIORITY         25
#define THREAD_STACK_SIZE       512
#define THREAD_TIMESLICE        5

#ifndef FINGERPRINT_UART_NAME
#define FINGERPRINT_UART_NAME "uart2"
#endif


#define EVENT_QUERY (1 << 1)

int fp_port(void)
{
    rt_thread_t fp_init_tid = RT_NULL;
    fp_init_tid = rt_thread_create("fp_init",
                            fp_init, FINGERPRINT_UART_NAME,
                            THREAD_STACK_SIZE,
                            THREAD_PRIORITY, THREAD_TIMESLICE);
    /* �������߳̿��ƿ飬��������߳� */
    if (fp_init_tid != RT_NULL)
        rt_thread_startup(fp_init_tid);
    return 0;
}
//INIT_ENV_EXPORT(fp_port);
MSH_CMD_EXPORT(fp_port, fp_port)



/*
 * ����˵����ɾ��һ��ָ��
 */
void fp_delet_char_cmd(int argc,char** argv)
{
    if (argc >= 2) {
        uint8_t code = fp_delet_char(atoi(argv[1]), 1);
        rt_kprintf("id:%d delet is %d !!\n", atoi(argv[1]), code);
    }else {
        rt_kprintf("Please enter the ID to verify!!\n");
    }
}

MSH_CMD_EXPORT(fp_delet_char_cmd, "fp_delet_char_cmd");


/*
 * ����˵����ɾ��ȫ��ָ��
 */
MSH_CMD_EXPORT(fp_empty, "fp_empty");

/*
 * ����˵������ȡָ��ģ�������Ϣ
 */
void fp_get_sys_para()
{
    SysPara p;
    fp_ack_type_t code = fp_read_sys_para(&p);

    rt_kprintf("code = %lx\n", code);
    rt_kprintf("p->state = %lx\n", p.state);
    rt_kprintf("p->sensor = %lx\n", p.sensor);
    rt_kprintf("p->capacity = %d\n", p.capacity);
    rt_kprintf("p->grade = %lx\n", p.grade);
    rt_kprintf("p->addr = %lx\n", p.addr);
    rt_kprintf("p->size = %lx\n", p.size);
    rt_kprintf("p->baud = %lx\n", p.baud);
}

MSH_CMD_EXPORT(fp_get_sys_para, "fp_read_sys_para");


/*
 * ����˵������ȡ��Чģ��ĸ���
 */
void fp_get_num(void)
{
    rt_uint16_t valid_n;
    fp_ack_type_t code = fp_valid_templete_num(&valid_n);
    rt_kprintf("code = %lx\n", code);
    rt_kprintf("valid_n = %lx,", valid_n);
}
MSH_CMD_EXPORT(fp_get_num, "fp_valid_templete_num");



/*
 * ��ȡ��Чid
 */
uint16_t get_eff_id(void)
{
    uint16_t count = 0;
    uint8_t page;
    IndexTable p;
    for (page = 0; page < 4; ++page) {
        fp_read_index_table(page,&p);
        for (int var = 0; var < 32; ++var) {
            for (int i = 0; i < 8; ++i) {
                if ((p.table[var] >> i) & 0x01) {
                    count++;
                }
                else {
//                    rt_kprintf("count = %d!!!\n",count);
                    return count;
                }
            }
        }
    }
    return -1;
}

MSH_CMD_EXPORT(get_eff_id, "get_eff_id");
/*
 * ��ȡid��״̬������ָ��
 */
uint8_t get_id_state(uint16_t page_id)
{
    uint8_t page = page_id / 256;
    IndexTable p;
    fp_read_index_table(page,&p);
    page = page_id % 256;
    return (p.table[page/8] >> (page%8)) & 0x01;
}
void get_id_state_cmd(int argc,char** argv)
{
    if (argc >= 2) {
        uint8_t var = get_id_state(atoi(argv[1]));
        rt_kprintf("id:%d  is %d !!\n", atoi(argv[1]), var);
    }else {
        rt_kprintf("Please enter the ID to verify!!\n");
    }
}

MSH_CMD_EXPORT(get_id_state_cmd, "get_id_state_cmd");


//ģ��Ӧ���ȷ������Ϣ����
//���ܣ�����ȷ���������Ϣ������Ϣ
//����: ensure
const char *EnsureMessage(uint8_t ensure)
{
    const char *p;
    switch(ensure)
    {
        case  0x00:
            p="OK";break;
        case  0x01:
            p="���ݰ����մ���";break;
        case  0x02:
            p="��������û����ָ";break;
        case  0x03:
            p="¼��ָ��ͼ��ʧ��";break;
        case  0x04:
            p="ָ��ͼ��̫�ɡ�̫��������������";break;
        case  0x05:
            p="ָ��ͼ��̫ʪ��̫��������������";break;
        case  0x06:
            p="ָ��ͼ��̫�Ҷ�����������";break;
        case  0x07:
            p="ָ��ͼ����������������̫�٣������̫С��������������";break;
        case  0x08:
            p="ָ�Ʋ�ƥ��";break;
        case  0x09:
            p="û������ָ��";break;
        case  0x0a:
            p="�����ϲ�ʧ��";break;
        case  0x0b:
            p="ID����ָ�ƿⷶΧ";
        case  0x0c:
            p="��ָ�ƿ��ģ��������Ч";
        case  0x0d:
            p="�ϴ�����ʧ��";
        case  0x0e:
            p="ģ�鲻�ܽ��ܺ������ݰ�";
        case  0x0f:
            p="�ϴ�ͼ��ʧ��";
        case  0x10:
            p="ɾ��ģ��ʧ��";break;
        case  0x11:
            p="���ָ�ƿ�ʧ��";break;
        case  0x12:
            p="���ܽ���͹���״̬";break;
        case  0x13:
            p="�����ȷ";break;
        case  0x14:
            p="ϵͳ��λʧ��";break;
        case  0x15:
            p="��������û����Чԭʼͼ��������ͼ��";break;
        case  0x16:
            p="��������ʧ��";break;
        case  0x17:
            p="����ָ�ƻ����βɼ�֮����ָû���ƶ���";break;
        case  0x18:
            p="��д FLASH ����";break;
        case  0x19:
            p="δ�������";break;
        case  0x1a:
            p="��Ч�Ĵ�����";break;
        case  0x1b:
            p="�Ĵ����趨���ݴ���";break;
        case  0x1c:
            p="���±�ҳ��ָ������";break;
        case  0x1e:
            p="�Զ�ע��ʧ��";break;
        case  0x1f:
            p="ָ�ƿ���";break;
        case  0x20:
            p="��ַ����";break;
        case  0x23:
            p="ָ��ģ��Ϊ��";break;
        case  0x24:
            p="ָ�ƿ�Ϊ��";break;
        case  0x26:
            p="�Զ�ע�ᳬʱ";break;
        case  0x27:
            p="�Զ�ע��ָ���Ѵ���";break;
        default :
            p=" ";break;
    }
 return p;
}


//��ʾȷ���������Ϣ
void ShowErrMessage(uint8_t ensure)
{
//    rt_kprintf((uint8_t*)EnsureMessage(ensure));
//    rt_kprintf("\n");
}

/*
 * ����˵�������ָ�ƣ������Ĵΰ�ѹ��������;��Ҫ̧����ָ
 */
void fp_add_fp(void)
{
    uint8_t i=0,ensure ,processnum=1,pressCout=1, str_buffer[40];
    uint16_t ID = get_eff_id();
    while(1)
    {
        switch (processnum)
        {
            case 0: //������ѹ4��ָ�Ʒֱ�浽4��charBuffer��
                i++;
                rt_sprintf((char*)str_buffer,"Please raise your finger: %d\n",pressCout);
                rt_kprintf((char*)str_buffer);
                ensure=fp_get_image();
                if(ensure==0x02)
                {
                    i=0;
                    processnum=1;//������һ��
                    pressCout++;
                    if(pressCout >=5)
                     {
                        pressCout = 0;
                        processnum=2;//�����ڶ���
                    }
                }
                break;

            case 1: //������ѹ4��ָ�Ʒֱ�浽4��charBuffer��
                i++;
                rt_sprintf((char*)str_buffer,"Please press your finger: %d\n",pressCout);
                rt_kprintf((char*)str_buffer);
                ensure=fp_get_image();
                if(ensure==0x00)
                {
                    i=0;
                    rt_thread_mdelay(100);//������Ҫ��ʱһ�£�ģ���ڲ�����ͼ����Ҫʱ��
                    ensure=fp_gen_char(pressCout);//��������
                    if(ensure==0x00)
                    {
                       processnum=0;//�������㲽
                    }else ShowErrMessage(ensure);
                }else ShowErrMessage(ensure);
                break;

            case 2:
                rt_kprintf("Generate fingerprint template!\n");
                ensure=fp_reg_model();
                if(ensure==0x00)
                {
                    rt_kprintf("Generate fingerprint template successfully!\n");
                    processnum=3;//����������
                }else {processnum=1;ShowErrMessage(ensure);}
                rt_thread_mdelay(1200);
                break;

            case 3:
                rt_kprintf("Fingerprint storage in %d!\n",ID);
                ensure=fp_store_char(CharBuffer1,ID);//����ģ��
                if(ensure==0x00)
                {
                    rt_kprintf("Fingerprint entered successfully!\n");
                    uint16_t ValidN;
                    fp_valid_templete_num(&ValidN);//����ָ�Ƹ���
//                    LCD_ShowNum(56,80,LB301Para.PS_max-ValidN,3,16);//��ʾʣ��ָ������
                    rt_thread_mdelay(1500);
                    return ;
                }else {processnum=1;ShowErrMessage(ensure);}
                break;
        }
        rt_thread_mdelay(200);
        if(i>=20)//����20��û�а���ָ���˳�
        {
            break;
        }
    }
}

MSH_CMD_EXPORT(fp_add_fp, "fp_valid_templete_num");

/*
 * ����˵����ˢָ��
 */

void fp_read(void)
{
    SearchResult p;
    fp_ack_type_t code;
    code = fp_auto_identify(&p);

    rt_kprintf("Find the fingerprint! code =%d page_id=%lx, mat_score=%lx", code, p.pageID, p.mathscore);
}
MSH_CMD_EXPORT(fp_read, "fp_valid_templete_num");


