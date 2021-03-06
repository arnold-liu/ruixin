#ifndef MAIN_OSD_H
#define MAIN_OSD_H





const char osd_tishi[]={0xe6, 0x8f, 0x90, 0xe7, 0xa4, 0xba, 0x00};
const char osd_success[]={0xe6, 0x88, 0x90, 0xe5, 0x8a, 0x9f, 0x00};//"成功";
const char osd_fail[]={0xe5, 0xa4, 0xb1, 0xe8, 0xb4, 0xa5, 0x00};//"失败";
const char osd_add[]={0xe5, 0xa2, 0x9e, 0xe5, 0x8a, 0xa0, 0x00};//"增加";
const char osd_del[]={0xe5, 0x88, 0xa0, 0xe9, 0x99, 0xa4, 0x00};//"删除";
const char osd_update[]={0xe4, 0xbf, 0xae, 0xe6, 0x94, 0xb9, 0x00};//"修改";

const char osd_return[]={0xe8, 0xbf, 0x94, 0xe5, 0x9b, 0x9e, 0x00};
const char osd_input_err[]={0xe8, 0xbe, 0x93, 0xe5, 0x85, 0xa5, 0xe9, 0x94, 0x99, 0xe8, 0xaf, 0xaf, 0x00};//"输入错误";

const char osd_no_sel[]={0xe8, 0xaf, 0xb7, 0xe5, 0x85, 0x88, 0xe9, 0x80, 0x89, 0xe6, 0x8b, 0xa9, 0x00};//"请先选择";


/*biao ding win*/
const char osd_cali_biaoqian[]={0xe6, 0xa0, 0x87, 0xe7, 0xad, 0xbe, 0x3a, 0x00};//"标签:";
const char osd_cali_biaozhun[]={0xe6, 0xa0, 0x87, 0xe5, 0x87, 0x86, 0xe5, 0xb0, 0xba, 0xe5, 0xaf, 0xb8, 0x3a, 0x00};//"标准尺寸:";
const char osd_cali_biaoding[]={0xe6, 0xa0, 0x87, 0xe5, 0xae, 0x9a, 0xe5, 0xb0, 0xba, 0xe5, 0xaf, 0xb8, 0x3a, 0x00};//"标定尺寸:";
const char osd_cali_jisuan[]={0xe6, 0xa0, 0x87, 0xe5, 0xae, 0x9a, 0xe5, 0xb0, 0xba, 0xe5, 0xaf, 0xb8, 0x3a, 0x00};//"标定尺寸:";
const char osd_cali_xiuzheng[]={0xe5, 0xae, 0x9e, 0xe6, 0xb5, 0x8b, 0xe5, 0xb0, 0xba, 0xe5, 0xaf, 0xb8, 0x3a, 0x00};//"实测尺寸:";
const char osd_cali_ad[]={0x61, 0x64, 0xe5, 0x80, 0xbc, 0x3a, 0x00};//"AD值:";



/*usr mgr win*/
const char osd_usrmgr_id[]={0xe5, 0xba, 0x8f, 0xe5, 0x8f, 0xb7, 0x00};//"序号";
const char osd_usrmgr_name[]={0xe5, 0x90, 0x8d, 0xe7, 0xa7, 0xb0, 0x00};//"名称";


/*mac mgr win*/
const char osd_macmgr_macname[]={0xe8, 0xbe, 0x93, 0xe5, 0x85, 0xa5, 0xe6, 0x9c, 0xba, 0xe5, 0x9e, 0x8b, 0xe5, 0x90, 0x8d, 0xe7, 0xa7, 0xb0, 0x3a, 0x00};//"输入机型名称:";
const char osd_macmgr_id[]={0xe5, 0xba, 0x8f, 0xe5, 0x8f, 0xb7, 0x00};//"序号";
const char osd_macmgr_name[]={0xe5, 0x90, 0x8d, 0xe7, 0xa7, 0xb0, 0x00};//"名称";


/*bg win*/
const char osd_mainwin_caidan[]={0xe8, 0x8f, 0x9c, 0xe5, 0x8d, 0x95, 0x00};
const char osd_mainwin_dangqianshijian[]={0xe5, 0xbd, 0x93, 0xe5, 0x89, 0x8d, 0xe6, 0x97, 0xb6, 0xe9, 0x97, 0xb4, 0x3a, 0x00};
const char osd_mainwin_jiqimingcheng[]={0xe6, 0x9c, 0xba, 0xe5, 0x99, 0xa8, 0xe5, 0x90, 0x8d, 0xe7, 0xa7, 0xb0, 0x00};
const char osd_mainwin_shishiad[]={0xe5, 0xae, 0x9e, 0xe6, 0x97, 0xb6, 0x41, 0x44, 0xe5, 0x80, 0xbc, 0x3a, 0x00};
const char osd_mainwin_jiaozhunbiaoqian[]={0xe6, 0xa0, 0xa1, 0xe5, 0x87, 0x86, 0xe6, 0xa0, 0x87, 0xe7, 0xad, 0xbe, 0x00};
const char osd_mainwin_ceshishuzhi[]={0xe6, 0xb5, 0x8b, 0xe8, 0xaf, 0x95, 0xe6, 0x95, 0xb0, 0xe5, 0x80, 0xbc, 0x00};
const char osd_mainwin_xiangshang[]={0xe5, 0x90, 0x91, 0xe4, 0xb8, 0x8a, 0x00};
const char osd_mainwin_xiangxia[]={0xe5, 0x90, 0x91, 0xe4, 0xb8, 0x8b, 0x00};
const char osd_mainwin_moshiqiehuan[]={0xe6, 0xa0, 0xa1, 0xe5, 0x87, 0x86, 0x00};//"校准";

const char osd_mainwin_celiang[]={0xe6, 0xb5, 0x8b, 0xe9, 0x87, 0x8f, 0x00};
const char osd_mainwin_jixing[]={0xe6, 0x9c, 0xba, 0xe5, 0x9e, 0x8b, 0x00};
const char osd_mainwin_shuzhi[]={0xe6, 0x95, 0xb0, 0xe5, 0x80, 0xbc, 0x00};
const char osd_mainwin_shijian[]={0xe6, 0x97, 0xb6, 0xe9, 0x97, 0xb4, 0x00};
const char osd_mainwin_wushuju[]={0xe6, 0x97, 0xa0, 0xe6, 0x95, 0xb0, 0xe6, 0x8d, 0xae, 0x00};
const char osd_mainwin_no2mmcali[]={0xe6, 0x97, 0xa0, 0x32, 0xe6, 0xaf, 0xab, 0xe7, 0xb1, 0xb3, 0xe6, 0xa0, 0xa1, 0xe5, 0x87, 0x86, 0xe8, 0xae, 0xb0, 0xe5, 0xbd, 0x95, 0x21, 0x00};
const char osd_mainwin_no4mmcali[]={0xe6, 0x97, 0xa0, 0x32, 0xe6, 0xaf, 0xab, 0xe7, 0xb1, 0xb3, 0xe6, 0xa0, 0xa1, 0xe5, 0x87, 0x86, 0xe8, 0xae, 0xb0, 0xe5, 0xbd, 0x95, 0x21, 0x00};
const char osd_mainwin_caliok[]={0xe6, 0xa0, 0xa1, 0xe5, 0x87, 0x86, 0xe6, 0x88, 0x90, 0xe5, 0x8a, 0x9f, 0x21, 0x00};
const char osd_mainwin_califail[]={0xe6, 0xa0, 0xa1, 0xe5, 0x87, 0x86, 0xe5, 0xa4, 0xb1, 0xe8, 0xb4, 0xa5, 0x21, 0x00};
const char osd_mainwin_getadcerr[]={0xe8, 0x8e, 0xb7, 0xe5, 0x8f, 0x96, 0x61, 0x64, 0xe5, 0x80, 0xbc, 0xe5, 0xa4, 0xb1, 0xe8, 0xb4, 0xa5, 0x21, 0x00};
const char osd_mainwin_calimode2[]={0x32, 0xe6, 0xaf, 0xab, 0xe7, 0xb1, 0xb3, 0xe6, 0xa0, 0xa1, 0xe5, 0x87, 0x86, 0xe6, 0xa8, 0xa1, 0xe5, 0xbc, 0x8f, 0x00};
const char osd_mainwin_calimode4[]={0x34, 0xe6, 0xaf, 0xab, 0xe7, 0xb1, 0xb3, 0xe6, 0xa0, 0xa1, 0xe5, 0x87, 0x86, 0xe6, 0xa8, 0xa1, 0xe5, 0xbc, 0x8f, 0x00};
const char osd_mainwin_measuremode[]={0xe6, 0xb5, 0x8b, 0xe9, 0x87, 0x8f, 0xe6, 0xa8, 0xa1, 0xe5, 0xbc, 0x8f, 0x00};//"测量模式"
const char osd_mainwin_2MMmeasuremode[]={'2', 'M', 'M', 0xe6, 0xb5, 0x8b, 0xe9, 0x87, 0x8f, 0xe6, 0xa8, 0xa1, 0xe5, 0xbc, 0x8f, 0x00};
const char osd_mainwin_4MMmeasuremode[]={'4', 'M', 'M', 0xe6, 0xb5, 0x8b, 0xe9, 0x87, 0x8f, 0xe6, 0xa8, 0xa1, 0xe5, 0xbc, 0x8f, 0x00};

const char osd_mainwin_nocalialert[]={0xe8, 0xbf, 0x98, 0xe6, 0xb2, 0xa1, 0xe6, 0x9c, 0x89, 0xe6, 0xa0, 0xa1, 0xe5, 0x87, 0x86, 0x2c, 0xe6, 0x98, 0xaf, 0xe5, 0x90, 0xa6, 0xe7, 0xbb, 0xa7, 0xe7, 0xbb, 0xad, 0x3f, 0x00};//"还没有校准,是否继续?";


/*try win*/
const char osd_try_warn[]={0xe8, 0xbd, 0xaf, 0xe4, 0xbb, 0xb6, 0xe8, 0xaf, 0x95, 0xe7, 0x94, 0xa8, 0xe7, 0xbb, 0x93, 0xe6, 0x9d, 0x9f, 0x2c, 0xe8, 0xaf, 0xb7, 0xe8, 0x81, 0x94, 0xe7, 0xb3, 0xbb, 0xe5, 0xbc, 0x80, 0xe5, 0x8f, 0x91, 0xe4, 0xba, 0xba, 0xe5, 0x91, 0x98, 0x21, 0x00};

/*enter win*/
const char osd_enter_user[]={0xe7, 0x94, 0xa8, 0xe6, 0x88, 0xb7, 0xe5, 0x90, 0x8d, 0x3a, 0x00};
const char osd_enter_pwd[]={0xe5, 0xaf, 0x86, 0xe7, 0xa0, 0x81, 0x3a, 0x00};
const char osd_enter_enter[]={0xe7, 0x99, 0xbb, 0xe5, 0xbd, 0x95, 0x00};
const char osd_enter_cancel[]={0xe5, 0x8f, 0x96, 0xe6, 0xb6, 0x88, 0x00};
const char osd_enter_err[]={0xe7, 0x94, 0xa8, 0xe6, 0x88, 0xb7, 0xe5, 0x90, 0x8d, 0xe6, 0x88, 0x96, 0xe5, 0xaf, 0x86, 0xe7, 0xa0, 0x81, 0xe9, 0x94, 0x99, 0xe8, 0xaf, 0xaf, 0x21, 0x00};
const char osd_enter_wrong[]={0xe9, 0x94, 0x99, 0xe8, 0xaf, 0xaf, 0x00};

/*menu win*/
const char osd_menu_reg[]={0xe6, 0xb3, 0xa8, 0xe5, 0x86, 0x8c, 0x00};
const char osd_menu_mac_mgr[]={0xe6, 0x9c, 0xba, 0xe5, 0x9e, 0x8b, 0xe7, 0xae, 0xa1, 0xe7, 0x90, 0x86, 0x00};//"机型管理";
const char osd_menu_export[]={0x55, 0xe7, 0x9b, 0x98, 0xe5, 0xaf, 0xbc, 0xe5, 0x87, 0xba, 0x00};
const char osd_menu_setup[]={0xe7, 0xb3, 0xbb, 0xe7, 0xbb, 0x9f, 0xe8, 0xae, 0xbe, 0xe7, 0xbd, 0xae, 0x00};//"系统设置"
const char osd_menu_cali[]={0xe6, 0xa0, 0x87, 0xe5, 0xae, 0x9a, 0xe8, 0xae, 0xbe, 0xe7, 0xbd, 0xae, 0x00};//"标定设置"
const char osd_menu_info[]={0xe4, 0xbd, 0xbf, 0xe7, 0x94, 0xa8, 0xe8, 0xaf, 0xb4, 0xe6, 0x98, 0x8e, 0x00};

/*reg win*/
const char osd_reg_user[]={0xe7, 0x94, 0xa8, 0xe6, 0x88, 0xb7, 0x3a, 0x00};
const char osd_reg_pwd[]={0xe5, 0xaf, 0x86, 0xe7, 0xa0, 0x81, 0x3a, 0x00};
const char osd_reg_addu[]={0xe6, 0xb7, 0xbb, 0xe5, 0x8a, 0xa0, 0xe7, 0x94, 0xa8, 0xe6, 0x88, 0xb7, 0x00};
const char osd_reg_mac[]={0xe6, 0x9c, 0xba, 0xe5, 0x9e, 0x8b, 0x3a, 0x00};
const char osd_reg_addm[]={0xe6, 0xb7, 0xbb, 0xe5, 0x8a, 0xa0, 0xe6, 0x9c, 0xba, 0xe5, 0x9e, 0x8b, 0x00};
const char osd_reg_return[]={0xe8, 0xbf, 0x94, 0xe5, 0x9b, 0x9e, 0x00};
const char osd_reg_errlen[]={0xe7, 0x94, 0xa8, 0xe6, 0x88, 0xb7, 0xe5, 0x90, 0x8d, 0xe5, 0x92, 0x8c, 0xe5, 0xaf, 0x86, 0xe7, 0xa0, 0x81, 0xe9, 0x83, 0xbd, 0xe4, 0xb8, 0x8d, 0xe8, 0x83, 0xbd, 0xe4, 0xb8, 0xba, 0xe7, 0xa9, 0xba, 0x21, 0x00};
const char osd_reg_errroleexist[]={0xe8, 0xaf, 0xa5, 0xe7, 0x94, 0xa8, 0xe6, 0x88, 0xb7, 0xe5, 0xb7, 0xb2, 0xe7, 0xbb, 0x8f, 0xe5, 0xad, 0x98, 0xe5, 0x9c, 0xa8, 0x21, 0x00};
const char osd_reg_addok[]={0xe6, 0xb7, 0xbb, 0xe5, 0x8a, 0xa0, 0xe6, 0x88, 0x90, 0xe5, 0x8a, 0x9f, 0x21, 0x00};
const char osd_reg_addfail[]={0xe6, 0xb7, 0xbb, 0xe5, 0x8a, 0xa0, 0xe5, 0xa4, 0xb1, 0xe8, 0xb4, 0xa5, 0x21, 0x00};

const char osd_reg_mgr[]={0xe7, 0xae, 0xa1, 0xe7, 0x90, 0x86, 0xe5, 0x91, 0x98, 0x00};//"管理员";

const char osd_reg_mcerrlen[]={0xe6, 0x9c, 0xba, 0xe5, 0x99, 0xa8, 0xe5, 0x90, 0x8d, 0xe4, 0xb8, 0x8d, 0xe8, 0x83, 0xbd, 0xe7, 0xa9, 0xba, 0x21, 0x00};
const char osd_reg_mcexist[]={0xe8, 0xaf, 0xa5, 0xe6, 0x9c, 0xba, 0xe5, 0x9e, 0x8b, 0xe5, 0xb7, 0xb2, 0xe7, 0xbb, 0x8f, 0xe5, 0xad, 0x98, 0xe5, 0x9c, 0xa8, 0x21, 0x00};

const char osd_reg_delu[]={0xe5, 0x88, 0xa0, 0xe9, 0x99, 0xa4, 0xe7, 0x94, 0xa8, 0xe6, 0x88, 0xb7, 0x00};//"删除用户"
const char osd_reg_delm[]={0xe5, 0x88, 0xa0, 0xe9, 0x99, 0xa4, 0xe6, 0x9c, 0xba, 0xe5, 0x9e, 0x8b, 0x00};//"删除机型"

const char osd_reg_delok[]={0xe5, 0x88, 0xa0, 0xe9, 0x99, 0xa4, 0xe6, 0x88, 0x90, 0xe5, 0x8a, 0x9f, 0x21, 0x00};//"删除成功!"
const char osd_reg_delfail[]={0xe5, 0x88, 0xa0, 0xe9, 0x99, 0xa4, 0xe5, 0xa4, 0xb1, 0xe8, 0xb4, 0xa5, 0x21, 0x00};//"删除失败!"


/*export win*/
const char osd_exp_warn[]={0xe8, 0xaf, 0xb7, 0xe5, 0x85, 0x88, 0xe6, 0x8a, 0x8a, 0x55, 0xe7, 0x9b, 0x98, 0xe6, 0xa0, 0xbc, 0xe5, 0xbc, 0x8f, 0xe5, 0x8c, 0x96, 0xe6, 0x88, 0x90, 0xe4, 0xb8, 0x80, 0xe4, 0xb8, 0xaa, 0x66, 0x61, 0x74, 0x33, 0x32, 0xe6, 0xa0, 0xbc, 0xe5, 0xbc, 0x8f, 0xe7, 0x9a, 0x84, 0xe5, 0x88, 0x86, 0xe5, 0x8c, 0xba, 0xef, 0xbc, 0x8c, 0xe5, 0x86, 0x8d, 0xe6, 0x89, 0xa7, 0xe8, 0xa1, 0x8c, 0xe5, 0xaf, 0xbc, 0xe5, 0x87, 0xba, 0xe3, 0x80, 0x82, 0x00};
const char osd_exp_ustat[]={0x55, 0xe7, 0x9b, 0x98, 0xe7, 0x8a, 0xb6, 0xe6, 0x80, 0x81, 0x3a, 0x00};
const char osd_exp_check[]={0xe6, 0xa3, 0x80, 0xe6, 0xb5, 0x8b, 0xe4, 0xb8, 0xad, 0x2e, 0x2e, 0x2e, 0x00};
const char osd_exp_export[]={0xe5, 0xaf, 0xbc, 0xe5, 0x87, 0xba, 0x00};
const char osd_exp_return[]={0xe8, 0xbf, 0x94, 0xe5, 0x9b, 0x9e, 0x00};
const char osd_exp_ustat_ok[]={0x55, 0xe7, 0x9b, 0x98, 0xe5, 0xb7, 0xb2, 0xe8, 0xbf, 0x9e, 0xe6, 0x8e, 0xa5, 0x00};//"U盘已连接";
const char osd_exp_machine[]={0xe6, 0x9c, 0xba, 0xe5, 0x9e, 0x8b, 0x00};//"机型";
const char osd_exp_time[]={0xe6, 0x97, 0xb6, 0xe9, 0x97, 0xb4, 0x00};//"时间";
const char osd_exp_all[]={0xe6, 0x89, 0x80, 0xe6, 0x9c, 0x89, 0x00};//"所有";


/*info win*/
//const char osd_info_msg[]={0xe6, 0x9a, 0x82, 0xe6, 0x97, 0xa0, 0xe8, 0xaf, 0xb4, 0xe6, 0x98, 0x8e, 0xe3, 0x80, 0x82, 0x00};


/*setup win*/
const char osd_setup_year[]={0xe5, 0xb9, 0xb4, 0x0};//"年";
const char osd_setup_mon[]={0xe6, 0x9c, 0x88, 0x0};//"月";
const char osd_setup_day[]={0xe6, 0x97, 0xa5, 0x0};//"日";
const char osd_setup_hour[]={0xe6, 0x97, 0xb6, 0x0};//"时";
const char osd_setup_min[]={0xe5, 0x88, 0x86, 0x0};//"分";
const char osd_setup_sec[]={0xe7, 0xa7, 0x92, 0x0};//"秒";
const char osd_setup_save[]={0xe4, 0xbf, 0x9d, 0xe5, 0xad, 0x98, 0x00};//"保存";

const char osd_setup_ok[]={0xe8, 0xae, 0xbe, 0xe7, 0xbd, 0xae, 0xe6, 0x88, 0x90, 0xe5, 0x8a, 0x9f, 0x00};//"设置成功";
const char osd_setup_fail[]={0xe8, 0xae, 0xbe, 0xe7, 0xbd, 0xae, 0xe5, 0xa4, 0xb1, 0xe8, 0xb4, 0xa5, 0x00};//"设置失败";















#endif
