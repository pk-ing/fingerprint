/*
 * @Author: Aurora-zk
 * @Date: 2021-03-14 12:39:09
 * @LastEditors: Aurora-zk
 * @LastEditTime: 2021-10-05 20:39:57
 * @contact: pk-ing@nyist.edu.cn
 * @FilePath: \fingerprint\fingerprint.h
 */

#ifndef _FINGERPRINT_H
#define _FINGERPRINT_H

#include "rtthread.h"
#include <rtdevice.h>
#include <rtdef.h>
#include <string.h>

#define HEAD_SIZE         2U
#define PREFIX_SIZE       9U

#define FP_HEAD_BIT       0U  /* ��ͷλ */
#define FP_ADDR_BIT       2U  /* ��ַλ */
#define FP_TOK_BIT        6U  /* ��ʶλ */
#define FP_LEN_BIT        7U  /* ����λ */

#define FP_INS_CMD_BIT    9U  /* ָ���ָ����λ */
#define FP_INS_PAR_BIT(n) ((n)+10U) /* ָ�������λ */
#define FP_INS_SUM_BIT(n) FP_INS_PAR_BIT(n) /* ָ���У���λ */

#define FP_REP_ACK_BIT(n) ((n)+9U)  /* Ӧ���ȷ����λ */
#define FP_REP_SUM_BIT(n) FP_REP_ACK_BIT(n) /* Ӧ���У���λ */


#define CharBuffer1 0x01
#define CharBuffer2 0x02
#define CharBuffer3 0x03
#define CharBuffer4 0x04

#define FP_HEAD_H     0xEF    /* ��ͷΪ 0xEF01 */
#define FP_HEAD_L     0x01
#define FP_ADDR_0     0xFF    /* Ĭ�ϵ�ַΪ 0xFFFFFFFF */
#define FP_ADDR_1     0xFF
#define FP_ADDR_2     0xFF
#define FP_ADDR_3     0xFF
#define FP_TOK_CMD    0x01
#define FP_TOK_DAT    0x02
#define FP_TOK_END    0x08

/* fp ϵ�������б� */
#define FP_CMD_GET_IMG              0x01    /* �Ӵ������϶���ͼ�����ͼ�񻺳��� */
#define FP_CMD_GEN_CHR              0x02    /* ����ԭʼͼ������ָ���������� CharBuffer1 �� CharBuffer2��CharBuffer1 �� CharBuffer2 */
//#define FP_CMD_MATCH      0x03 /* ��ȷ�ȶ� CharBuffer1 �� CharBuffer2 �е������ļ� */
#define FP_CMD_SEARCH               0x04    /* �������ļ����������򲿷�ָ�ƿ� */
#define FP_CMD_REG_MODEL            0x05    /* �����ɵ������ļ��ϲ�������ģ����������ļ������� */
#define FP_CMD_STR_CHR              0x06    /* �������������е��ļ����浽 flash ָ�ƿ��� */
#define FP_CMD_FINGER_CHAR_UP       0x07    /* �����ϴ� */                                                      //����
#define FP_CMD_FINGER_CHAR_DOWN     0x08    /* �������� */                                                      //����
//#define FP_CMD_DOWN_CHR   0x09 /* ����λ������һ�������ļ������������� */
#define FP_CMD_UP_IMG               0x0A    /* �ϴ�ԭʼͼ�� */                                                      //����
//#define FP_CMD_DOWN_IMG   0x0B /* ����ԭʼͼ�� */
#define FP_CMD_DEL_CHR    0x0C /* ɾ�� flash ָ�ƿ��е�һ�������ļ� */
#define FP_CMD_EMPTY      0x0D /* ��� flash ָ�ƿ� */
#define FP_CMD_FINGER_MOUDLE_SET    0x0E    /* ����ģ����� */
#define FP_CMD_RD_PARAM   0x0F /* ��ϵͳ�������� */
//#define FP_CMD_ENROLL     0x10 /* ע��ģ�� */
//#define FP_CMD_IDENTIFY   0x11 /* ��ָ֤�� */
#define FP_CMD_SET_PWD              0x12    /* �����豸���ֿ��� */
#define FP_CMD_VFY_PWD              0x13    /* ��֤�豸���ֿ��� */
//#define FP_CMD_GET_RAMD   0x14 /* ��������� */
//#define FP_CMD_SET_ADDR   0x15 /* ����оƬ��ַ */
#define FP_CMD_READ_SYS_PARA        0x16    /* ��ȡϵͳ�������� */
//#define FP_CMD_PORT_CNTL  0x17 /* ͨѶ�˿ڣ�UART/USB�����ؿ��� */
//#define FP_CMD_WR_NOTEPAD 0x18 /* д���±� */
//#define FP_CMD_RD_NOTEPAD 0x19 /* �����±� */
//#define FP_CMD_BURN_CODE  0x1A /* ��дƬ�� FLASH */
//#define FP_CMD_HS_SEARCH  0x1B /* �������� FLASH */
//#define FP_CMD_GEN_BIN_IM 0x1C /* ���ɶ�ֵ��ָ��ͼ�� */

#define FP_CMD_VALID_TEMPLETE_NUM   0x1D    /* ��ȡ��Чģ����� */
#define FP_CMD_READ_INDEX_TABLE     0x1F    /* �������� */

#define FP_CMD_GET_RNROLL_IMG       0x29    /* ע���û�ȡͼ�� */
#define FP_CMD_CANCEL               0x30    /* ȡ��ָ�� */
#define FP_CMD_AUTO_ENROLL          0x31    /* �Զ�ע��ģ�� */
#define FP_CMD_AUTO_IDENTIFY        0x32    /* �Զ���ָ֤�� */
#define FP_CMD_SLEEP                0x33    /* ����ָ�������ʹ��0X60�� */
#define FP_CMD_GET_CHIP_UID         0x34    /* ��ȡоƬ���к�  */
#define FP_CMD_GET_CHIP_ECHO        0x35    /* ��������  */
#define FP_CMD_AUTO_CAI_SENSOR      0x36    /* У�鴫����  */

/**
 * @brief ������ָ��Ӧ���ȷ����
 * 
 */
enum fp_ack_type
{
    CMD_OK        = 0x00, /* ��ʾָ��ִ����ϻ� OK */
    DAT_ERR       = 0x01, /* ��ʾ���ݰ����մ��� */
    NO_FINGER     = 0x02, /* ��ʾ��������û����ָ */
//    INPUT_FIN     = 0x03, /* ��ʾ¼��ָ��ͼ��ʧ�� */
//    TOO_DRY       = 0x04, /* ��ʾָ��ͼ��̫�ɡ�̫�������������� */
//    TOO_WET       = 0x05, /* ��ʾָ��ͼ��̫ʪ��̫�������������� */
    TOO_MESS      = 0x06, /* ��ʾָ��ͼ��̫�Ҷ����������� */
    FEATURE_PT    = 0x07, /* ��ʾָ��ͼ����������������̫�٣������̫С�������������� */
//    FINGER_MIS    = 0x08, /* ��ʾָ�Ʋ�ƥ�� */
    SERCH_ERR     = 0x09, /* ��ʾû������ָ�� */
    MERGE_FAIL    = 0x0A, /* ��ʾ�����ϲ�ʧ�� */
    OVER_SIZE     = 0x0B, /* ��ʾ����ָ�ƿ�ʱ��ַ��ų���ָ�ƿⷶΧ */
//    RD_TMPL_ERR   = 0x0C, /* ��ʾ��ָ�ƿ��ģ��������Ч */
//    UP_FEAT_ERR   = 0x0D, /* ��ʾ�ϴ�����ʧ�� */
//    NO_SUBSEQ_PKG = 0x0E, /* ��ʾģ�鲻�ܽ��ܺ������ݰ� */
//    UP_IMG_ERR    = 0x0F, /* ��ʾ�ϴ�ͼ��ʧ�� */
    DEL_TMPL_ERR  = 0x10, /* ��ʾɾ��ģ��ʧ�� */
    CLR_FP_ERR    = 0x11, /* ��ʾ���ָ�ƿ�ʧ�� */
//    LP_MODE_ERR   = 0x12, /* ��ʾ���ܽ���͹���״̬ */
    PWD_ERR       = 0x13, /* ��ʾ�����ȷ */
//    SYS_RST_ERR   = 0x14, /* ��ʾϵͳ��λʧ�� */
    BUF_NO_IMG    = 0x15, /* ��ʾ��������û����Чԭʼͼ��������ͼ�� */
//    OTA_ERR       = 0x16, /* ��ʾ��������ʧ�� */
    RESIDUAL_FP   = 0x17, /* ��ʾ����ָ�ƻ����βɼ�֮����ָû���ƶ��� */
    RW_FLASH_ERR  = 0x18, /* ��ʾ��д FLASH ���� */
    UNDEF_ERR     = 0x19, /* δ������� */
    INVALID_REG   = 0x1A, /* ��Ч�Ĵ����� */
    CFG_REG_ERR   = 0x1B, /* �Ĵ����趨���ݴ���� */
//    NP_PAGE_ERR   = 0x1C, /* ���±�ҳ��ָ������ */
//    PORT_CNTL_ERR = 0x1D, /* �˿ڲ���ʧ�� */
    ENROLL_ERR    = 0x1E, /* �Զ�ע�ᣨenroll��ʧ�� */
    FP_FULL       = 0x1F, /* ָ�ƿ��� */
    ADDR_ERR    = 0x20, /* ��ַ���� */
    FP_TEM_NO_EMPTY    = 0x22, /* ָ��ģ��ǿ� */
    FP_TEM_EMPTY    = 0x22, /* ָ��ģ��Ϊ�� */
    FP_LIB_EMPTY      = 0x24, /* ָ�ƿ�Ϊ�� */
    INPUT_TIMES_SET_ERR      = 0x24, /* ¼��������ô��� */
    OVERTIME      = 0x26, /* ��ʱ */
    FP_EXIST      = 0x27, /* ָ���Ѵ��� */

//    SUBSEQ_OK     = 0xF0, /* �к������ݰ���ָ���ȷ���պ��� 0xf0 Ӧ�� */
//    SUBSEQ_CMD    = 0xF1, /* �к������ݰ���ָ�������� 0xf1 Ӧ�� */
//    BURN_SUM_ERR  = 0xF2, /* ��ʾ��д�ڲ� FLASH ʱ��У��ʹ��� */
//    BURN_TOK_ERR  = 0xF3, /* ��ʾ��д�ڲ� FLASH ʱ������ʶ���� */
//    BURN_LEN_ERR  = 0xF4, /* ��ʾ��д�ڲ� FLASH ʱ�������ȴ��� */
//    BURN_COD_EXSZ = 0xF5, /* ��ʾ��д�ڲ� FLASH ʱ�����볤��̫�� */
//    BURN_FUNC_ERR = 0xF6, /* ��ʾ��д�ڲ� FLASH ʱ����д FLASH ʧ�� */
};

typedef enum fp_ack_type fp_ack_type_t;

typedef struct
{
    uint16_t pageID;    //ָ��ID
    uint16_t mathscore; //ƥ��÷�
}SearchResult;

typedef struct
{
    rt_uint16_t state;      //1��״̬�Ĵ���
    rt_uint16_t sensor;     //2������������  0��fpc1011c��2����Ⱥ c500��3����Ⱥ s500 ��״��7������о΢��״��9���û��Զ��崫������
	rt_uint16_t capacity;   //3��ָ�ƿ�����
	rt_uint16_t grade;      //4����ȫ�ȼ�  1/2/3/4/5
	rt_uint32_t addr;       //5���豸��ַ
	rt_uint16_t size;       //6�����ݰ���С    0��32bytes 1��62bytes 2��128bytes 3��256bytes
	rt_uint16_t baud;       //7������������    (������Ϊ 9600*N bps)
}SysPara;

typedef struct
{
    uint8_t table[32];
}IndexTable;

/**
 * @brief Get the image object
 *        ��ȡ������ͼ��
 * 
 * @return fp_ack_type_t ģ��ȷ����
 */
fp_ack_type_t fp_get_image(void);

/****************************************************************
 *  ��������� BufferID(������������)
 *  ���ز����� ȷ����
 * ****************************************************************/
fp_ack_type_t fp_gen_char(rt_uint8_t buff_id);

/*
 * ����˵���� �� CharBuffer1 �� CharBuffer2 �е������ļ����������򲿷�ָ�ƿ⡣�����������򷵻�ҳ�롣
 * ��������� BufferID�� StartPage(��ʼҳ)��PageNum��ҳ����
 *          page_id : ҳ�루����ָ��ģ�壩
 *          mat_score : �ȶԵ÷�
 * ���ز����� ȷ����
 */
fp_ack_type_t fp_search(uint8_t buffer_id, uint16_t startpage, uint16_t page_num, SearchResult *p);

/*
 * ����˵���� �� CharBuffer1 �� CharBuffer2 �е������ļ��ϲ�����ģ�壬������� CharBuffer1 �� CharBuffer2��
 * ��������� none
 * ���ز����� ȷ����
 */
fp_ack_type_t fp_reg_model(void);

/*
 * ����˵���� �� CharBuffer1 �� CharBuffer2 �е�ģ���ļ��浽 PageID ��flash ���ݿ�λ�á�
 * ��������� BufferID(��������)��PageID��ָ�ƿ�λ�úţ�
 * ���ز����� ȷ����
 */
fp_ack_type_t fp_store_char(rt_uint8_t buff_id, rt_uint16_t page_id);

/*
 * ����˵���� ɾ�� flash ���ݿ���ָ�� ID �ſ�ʼ�� N ��ָ��ģ��
 * ��������� PageID(ָ�ƿ�ģ���)��N ɾ����ģ�������
 * ���ز����� ȷ����
 */
fp_ack_type_t fp_delet_char(rt_uint16_t page_id, rt_uint16_t n);

/*
 * ����˵���� ɾ�� flash ���ݿ�������ָ��ģ��
 * ��������� none
 * ���ز����� ȷ����
 */
fp_ack_type_t fp_empty(void);

/*
 * ����˵���� дģ��Ĵ���
 * ��������� �Ĵ������ д������
 * ���ز����� ȷ����
 */
fp_ack_type_t fp_set_reg(rt_uint8_t register_number, rt_uint8_t content);
/*
 * ����˵���� дģ��Ĵ���
 * ��������� �Ĵ������ д������
 * ���ز����� ȷ����
 */
fp_ack_type_t fp_write_reg(rt_uint8_t register_number, rt_uint8_t content);

/*
 * ����˵���� ��ȡģ��Ļ��������������ʣ�����С�ȣ���
 * ���������
 * ���ز����� ȷ����
 */
fp_ack_type_t fp_read_sys_para(SysPara *p);

/**
 * @brief verify password
 *        ��֤����
 * 
 * @param param 
 * @return rt_err_t 
 */
rt_err_t fp_vfy_pwd(void);

/*
 * ����˵������ָ֤��
 * ���������
 * ���ز�����
 */
fp_ack_type_t fp_auto_identify(SearchResult *p);

/*
 * ����˵���� ����Чģ�����
 * ��������� none
 * ���ز����� ȷ����
 */
fp_ack_type_t fp_valid_templete_num(rt_uint16_t *valid_n);

/*
 * ����˵���� ��������
 * ��������� none
 * ���ز����� ȷ����
 */
fp_ack_type_t fp_read_index_table(uint8_t Page, IndexTable *p);



/* �ϲ� API */


/**
 * @brief ģ���ʼ������
 * 
 * @param name 
 */
void fp_init(void *name);






#endif
