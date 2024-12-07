#include "App_UI.h"

/* ��ʾ���������  */
MainSet_Str MainMenu_Parameter[] = {
    {"Setting",7,0},
    {"Clock",5,0},
    {"Timer",5,0},
    {"Calendar",8,0},
    {"Weather",7,0},
    {"Games",5,0},
};


MainSet_Str_All Str_AllArray[] = {
    {{{"aetting",7,0},{"alock",5,0},{"aimer",5,0},{"aalendar",8,0},},
        0,3,4,4,12,12,0,0,0,0,0,14,2,7,2,2,0},

    {{{"betting",7,0},{"block",5,0},{"bimer",5,0},{"balendar",8,0},{"beather",7,0},{"bames",5,0},},
        0,5,4,4,12,12,0,0,0,0,0,14,7,7,2,2,0},

    {{{"cetting",7,0},{"clock",5,0},{"cimer",5,0},{"calendar",8,0},{"ceather",7,0},{"cames",5,0},},
        0,5,4,4,12,12,0,0,0,0,0,14,7,7,2,2,0},

    {{{"detting",7,0},{"dlock",5,0},{"cimer",5,0},{"dalendar",8,0},{"eeather",7,0},{"fames",5,0},},
        0,5,4,4,12,12,0,0,0,0,0,14,7,7,2,2,0},

    {{{"eetting",7,0},{"elock",5,0},{"cimer",5,0},{"dalendar",8,0},{"eeather",7,0},{"fames",5,0},},
        0,5,4,4,12,12,0,0,0,0,0,14,7,7,2,2,0},

    {{{"fetting",7,0},{"flock",5,0},{"cimer",5,0},{"dalendar",8,0},{"eeather",7,0},{"fames",5,0},},
        0,5,4,4,12,12,0,0,0,0,0,14,7,7,2,2,0},
};

Menu_Operate Table[] = {
    //��ʼ�˵�
    {0,7,8,1,0,ui_show},
    //1���˵�
    {1,7,8,9,0,String_show},
    {2,7,8,9,0,String_show},
    {3,7,8,9,0,String_show},
    {4,7,8,9,0,String_show},
    {5,7,8,9,0,String_show},
    {6,7,8,9,0,String_show},
    //�������
    {7,7,7,7,7,Str_Coordinate_Add},
    {8,8,8,8,8,Str_Coordinate_Decrease},
    //2���˵�
    {9,9,9,9,9,Str_Operate}
};


u8g2_t u8g2;
//�˵�����ѡ��
volatile int Select_flag; // ���� һ�� ˫�� ����
//����ui�����ƶ��ٶ� ֵ�����2�ı���
int speed = 4;
int c_speed = 2;
//�˵���ʾͼƬ����
int MainMenu_Select = 1;
int MainMenu_Picture_x = -22;
int MainMenu_Picture_x_target = -22;
int MainMenu_MaxNum = 5;//�˵���ʾ������� ��0��ʼ����
//�˵���ʾ�ַ����ֲ���
int MainMenu_Str_y = 78;
int MainMenu_Str_x = 34;
int MainMenu_Str_y_target = 64;
//�˵���൯��
int MainMenu_Rec_x = -6;
int MainMenu_Rec_x_target = 0;

int func_index = 0;
int func_index_last = 0;
void (*current_operation_index)(void);

int run_str(int* now, int* trag, const int speed, const int c_speed) {
    int temp = 0;
    if (*now > *trag)
    {
        temp = ((*now - *trag) > c_speed) ? speed : c_speed;
        *now -= temp;
    }
    else if (*now < *trag)
    {
        temp = ((*trag - *now) > c_speed) ? speed : c_speed;
        *now += temp;
    }
    else
    {
        return 1;
    }
    return 0;
}


void ui_show(void) {
    char i = 0;
    u8g2_SetFont(&u8g2, u8g2_font_t0_22b_tf);//���ò˵��ַ�������ʾ����

    /*  ��ʾ�����  */
    for (i = 0;i <= MainMenu_MaxNum;i++) { //��ʾ�˵�ͼƬ
        u8g2_DrawXBMP(&u8g2, MainMenu_Picture_x + i * 64, 0, 44, 44, logo[i]);
    }

    if (MainMenu_Select >= 0 && MainMenu_Select <= MainMenu_MaxNum) {//��ʾ�˵����ַ�����
        u8g2_DrawStr(&u8g2, MainMenu_Str_x, MainMenu_Str_y, MainMenu_Parameter[MainMenu_Select].str);
    }
    u8g2_DrawBox(&u8g2, MainMenu_Rec_x, 50, 8, 16);//��ߵ�������

    /*  ������������  */
    run_str(&MainMenu_Picture_x, &MainMenu_Picture_x_target, 32, c_speed);//��ͼƬ��������ƶ�
    run_str(&MainMenu_Str_y, &MainMenu_Str_y_target, speed, c_speed);//��ͼƬ�ַ�������������ƶ�
    run_str(&MainMenu_Rec_x, &MainMenu_Rec_x_target, speed, c_speed);//��������
}


void String_show(void) {
    char i = 0;
    char temp = func_index - 1;
    Str_AllArray[temp].rate_y_duan = 64 / (Str_AllArray[temp].len + 1);

    u8g2_SetFont(&u8g2, u8g2_font_6x13_te);
    //��ʾ�ַ�
    for (i = 0; i <= Str_AllArray[temp].len; i++)
    {
        u8g2_DrawStr(&u8g2, Str_AllArray[temp].str_x, Str_AllArray[temp].str_y + i * 16, Str_AllArray[temp].Str_Pa[i].str);
        u8g2_DrawFrame(&u8g2, 90, Str_AllArray[temp].str_y + i * 16 - 8, 9, 9);//��Ӧ�˵��»��տ�
    }

    //��ѡ����
    //u8g2_DrawVLine(&u8g2, 124, 0, (Str_AllArray[temp].len/4+1)*64);
    u8g2_DrawRBox(&u8g2, 122, Str_AllArray[temp].rate_y, 6, Str_AllArray[temp].rate_y_duan, 2);
    u8g2_DrawRFrame(&u8g2, 122, 1, 6, 62, 2);
    u8g2_SetDrawColor(&u8g2, 2);
    //��ѡ���
    Str_AllArray[temp].fram_w_target = 7 * Str_AllArray[temp].Str_Pa[Str_AllArray[temp].select].num;
    u8g2_DrawRBox(&u8g2, Str_AllArray[temp].fram_x, Str_AllArray[temp].fram_y, Str_AllArray[temp].fram_w, Str_AllArray[temp].fram_h, 4);
    u8g2_SetDrawColor(&u8g2, 1);

    //�������˵�ȷ����ɨ���ĸ�һ���������ˣ���������Լ����Լ���ִ�еĴ���
    for (i = 0; i <= Str_AllArray[temp].len; i++) {
        //���԰���
        if (temp == 1)
        {
            if (1 == Str_AllArray[temp].Str_Pa[i].flag)
            {
                u8g2_DrawBox(&u8g2, 90, Str_AllArray[temp].str_y + i * 16 - 8, 9, 9);//��Ӧ���²�������ʾʵ�ķ���	
                switch (i)
                {
                case 0:
                    // LED1_ON();
                    break;
                case 1:
                    // LED2_ON();
                    break;
                default:
                    // LED1_ON();
                    // LED2_ON();
                    break;
                }
            }
            else
            {
                switch (i)
                {
                case 0:
                    // LED1_OFF();
                    break;
                case 1:
                    // LED2_OFF();
                    break;
                }
            }
        }
    }

    run_str(&Str_AllArray[temp].str_y, &Str_AllArray[temp].str_y_target, speed, c_speed);
    run_str(&Str_AllArray[temp].fram_y, &Str_AllArray[temp].fram_y_target, speed, c_speed);
    run_str(&Str_AllArray[temp].fram_w, &Str_AllArray[temp].fram_w_target, 3, 1);
    run_str(&Str_AllArray[temp].rate_y, &Str_AllArray[temp].rate_y_target, speed, c_speed);
}


void Str_Coordinate_Add(void) {
    //���������������
    if (func_index_last == 0) { //ͼƬ�����ƶ�
        MainMenu_Select += 1;
        MainMenu_Picture_x_target -= 64;
        if (MainMenu_Select >= 0 && MainMenu_Select <= MainMenu_MaxNum) {
            //ģ���ַ�����Ļ�·�����Ч��		
            MainMenu_Str_y = 78;
            MainMenu_Str_y_target = 64;
            //ģ����ߵ���Ч��������ѡ���������Ч��
            MainMenu_Rec_x = -8;
            MainMenu_Rec_x_target = 0;
            // Beep_Work(Turn_ON);//���з���������
        }
        /*    ��ֵ�����    */
        if (MainMenu_Picture_x_target >= 42) {//����������һ��ͼƬ��ֵ

            MainMenu_Picture_x_target = 42;
        }
        else if (MainMenu_Picture_x_target <= (-MainMenu_MaxNum * 64 + 42)) {//�����ұ����һ��ͼƬ��ֵ
            MainMenu_Picture_x_target = (-MainMenu_MaxNum * 64 + 42);
        }
        if (MainMenu_Select < 0) {//���в˵�ѡ��ֵ��ֵ
            MainMenu_Select = 0;
        }
        else if (MainMenu_Select >= MainMenu_MaxNum) {
            MainMenu_Select = MainMenu_MaxNum;
        }
    }
    //��1���˵��������
    if (func_index_last >= 1 && func_index_last <= 6) {
        char temp = func_index_last - 1;
        Str_AllArray[temp].select++;

        if (Str_AllArray[temp].select >= 0 && Str_AllArray[temp].select <= Str_AllArray[temp].len) {
            // Beep_Work(Turn_ON);//���з���������
            Str_AllArray[temp].rate_y_target += Str_AllArray[temp].rate_y_duan;
            Str_AllArray[temp].fram_select = Str_AllArray[temp].select;

            if (Str_AllArray[temp].fram_select >= 0 && Str_AllArray[temp].fram_select <= 3) {//�Ƿ�Կ����ƶ�

                Str_AllArray[temp].fram_y_target += 16;
            }
            else if (Str_AllArray[temp].select <= Str_AllArray[temp].len) {//�Ƿ�����ַ����ƶ�
                Str_AllArray[temp].str_y_target -= 16;
            }

            Str_AllArray[temp].fram_x_target = 7 * Str_AllArray[temp].Str_Pa[Str_AllArray[temp].select].num;
        }

        if (Str_AllArray[temp].select > Str_AllArray[temp].len) {//	������ֵ
            Str_AllArray[temp].select = Str_AllArray[temp].len;
        }

    }
    if (func_index == 7 || func_index == 8) {//ִ������Ӽ��������ڻص�֮ǰ������ֵ
        func_index = func_index_last;
    }

}
void Str_Coordinate_Decrease(void) {
    if (func_index_last == 0) {//�����˵�������в���

        MainMenu_Select -= 1;
        MainMenu_Picture_x_target += 64;
        if (MainMenu_Select >= 0 && MainMenu_Select <= MainMenu_MaxNum) {
            MainMenu_Str_y = 78;
            MainMenu_Str_y_target = 64;

            MainMenu_Rec_x = -8;
            MainMenu_Rec_x_target = 0;
            // Beep_Work(Turn_ON);//���з���������
        }
        /*    ��ֵ�����    */
        if (MainMenu_Picture_x_target >= 42) {//����������һ��ͼƬ��ֵ
            MainMenu_Picture_x_target = 42;
        }
        else if (MainMenu_Picture_x_target <= (-MainMenu_MaxNum * 64 + 42)) {//�����ұ����һ��ͼƬ��ֵ
            MainMenu_Picture_x_target = (-MainMenu_MaxNum * 64 + 42);
        }
        if (MainMenu_Select < 0) {//���в˵�ѡ��ֵ��ֵ
            MainMenu_Select = 0;
        }
        else if (MainMenu_Select >= MainMenu_MaxNum) {
            MainMenu_Select = MainMenu_MaxNum;
        }
    }
    //��һ���˵��������
    if (func_index_last >= 1 && func_index_last <= 6) {
        char temp = func_index_last - 1;
        Str_AllArray[temp].select--;

        if (Str_AllArray[temp].select >= 0 && Str_AllArray[temp].select <= Str_AllArray[temp].len) {
            // Beep_Work(Turn_ON);//���з���������
            Str_AllArray[temp].rate_y_target -= Str_AllArray[temp].rate_y_duan;
            Str_AllArray[temp].fram_select = Str_AllArray[temp].select;
            if (Str_AllArray[temp].fram_select >= 0 && Str_AllArray[temp].fram_select < 3) {//�Ƿ�Կ����ƶ�
                Str_AllArray[temp].fram_y_target -= 16;
            }
            else if (Str_AllArray[temp].select > 0) {//�Ƿ�����ַ����ƶ�
                Str_AllArray[temp].str_y_target += 16;
            }
        }
        if (Str_AllArray[temp].select < 0) {	//	������ֵ
            Str_AllArray[temp].select = 0;
        }
    }

    if (func_index == 7 || func_index == 8) {//ִ������Ӽ��������ڻص�֮ǰ������ֵ
        func_index = func_index_last;
    }
}


//	�����˵���������ѡ��ִ��,����Ӧһ���˵������MainSet_Str��flag�Ϳ��Ե�֪������ĸ�
void Str_Operate(void) {
    int index = func_index_last - 1;

    if (Str_AllArray[index].Str_Pa[Str_AllArray[index].select].flag == 1) {
        Str_AllArray[index].Str_Pa[Str_AllArray[index].select].flag = 0;
    }
    else if (Str_AllArray[index].Str_Pa[Str_AllArray[index].select].flag == 0) {
        Str_AllArray[index].Str_Pa[Str_AllArray[index].select].flag = 1;
    }

    //ִ�ж����˵������������ڻص�֮ǰ������ֵ
    func_index = func_index_last;
}
void App_Menu_UI(void) {
    func_index_last = func_index;
    if (Select_flag > 0) {
        switch (Select_flag) {
        case 1: func_index = Table[func_index].upper;break;
        case 2: func_index = Table[func_index].next;break;
        case 3: func_index = Table[func_index].enter;
            if (func_index == 1) {
                func_index += MainMenu_Select;
            }
            break;
        case 4: func_index = Table[func_index].back;break;
        case 5: func_index = 0;break;
        default: break;
        }
        Select_flag = 0;
    }
    if (func_index == 7 || func_index == 8 || func_index == 9) {
        current_operation_index = Table[func_index].current_operation;
        (*current_operation_index)();
    }
    current_operation_index = Table[func_index].current_operation;
    (*current_operation_index)();

    u8g2_SendBuffer(&u8g2);
    u8g2_ClearBuffer(&u8g2);
}


void App_UI_Init(void) {
    u8g2Init(&u8g2);
    Inf_Key_Init();
    Inf_LED_Init();
}
