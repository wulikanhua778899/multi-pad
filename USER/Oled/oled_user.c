#include "oled_user.h"
#include "oled.h"
#include "flash_user.h"
#include "MatrixKeys.h"
#include "oled_42.h"

extern uint8_t menu_change_lock;

void load_menu_from_flash(uint8_t folderIndex);

void oled_show_pic(uint8_t menu_index);

static void screen_shake(uint8_t screen_index, uint8_t *pre, uint8_t mode);

// ͼƬ�洢����
uint8_t oled_42_array[OLED_42_NUM][SIZE_42];
uint8_t oled_96_array[OLED_96_NUM][SIZE_96];

// ��������
uint8_t random_refresh[3][20] = {7, 14, 17, 10, 11, 3, 18, 5, 13, 15, 9, 1, 0, 2, 6, 8, 16, 4, 19, 12,
                                 9, 12, 6, 8, 4, 19, 0, 7, 17, 3, 13, 15, 1, 16, 10, 11, 2, 14, 5, 18,
                                 3, 18, 15, 13, 16, 10, 4, 1, 0, 14, 7, 17, 11, 5, 9, 19, 12, 6, 2, 8};

// �ڰ���Ļ����
static unsigned char black_background[360] = {0x00};
static unsigned char white_background[360] = {
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};

// ͼƬ��ʼ
unsigned char bmp1[360] = {
        0x00,0x60,0x60,0x60,0x60,0xa0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xc1,0xe0,0xf0,0xf8,0xfc,0xfc,0xfe,0xfe,0xff,0xff,0xff,0xff,0xff,0x7f,0x7f,0x7f,0x3f,0x3f,0x3f,0x3f,0x3f,0x1f,0x1f,0x1f,0x1f,0x1f,0x3f,0x3f,0x3f,0x3f,0x7f,0x7f,0x7f,0xff,0xff,0xff,0xff,0xfe,0xfc,0xfc,0xf8,0xf0,0xe0,0xc0,0x80,0x00,0x08,0x10,0x20,0x40,0x80,0x0f,0x7f,0xff,0xff,0xff,0xff,0xff,0x00,0x08,0x08,0x00,0x00,0x00,0x80,0x00,0xc0,0xf0,0xf8,0xfe,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x3f,0x3f,0x1f,0x07,0x81,0xe1,0x20,0x00,0x08,0x00,0x04,0x02,0x02,0x02,0x01,0x01,0x01,0x01,0x01,0x81,0xc1,0xc1,0xc1,0x81,0x00,0x02,0x02,0x00,0x00,0x01,0x03,0x03,0x07,0x1f,0xff,0x3f,0xff,0xff,0xff,0xff,0xfc,0xf8,0xe0,0x80,0x00,0x00,0x00,0x01,0x07,0x0f,0x3f,0x7f,0x00,0x00,0x00,0xc0,0xfc,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfe,0x18,0x00,0x00,0x00,0x07,0x7f,0xff,0x7f,0x08,0x00,0x00,0x00,0x00,0x58,0xd8,0x00,0xc4,0xfc,0xfc,0xfc,0xfe,0xff,0xff,0xff,0xff,0xc5,0x00,0x58,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x1f,0xff,0xff,0xff,0xff,0xff,0xff,0xfc,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7f,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x0e,0x0f,0x0e,0xe0,0xc0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x50,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x1c,0x58,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x1c,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x80,0xf0,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x3f,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfe,0xec,0x08,0x00,0x00,0x00,0x00,0x00,0x40,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x60,0xe0,0xf8,0xf8,0xfc,0xfe,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xbf,0xcf,0xf1,0xfc,0x3f,0x07,0x00,0x00,0x00,0x00
};
// ͼƬ����

// ͼƬ��ʼ
unsigned char bmp2[360] = {
        0xff,0xff,0xff,0xff,0xfe,0xfc,0xfd,0xff,0x7e,0x7e,0xff,0xff,0xff,0xff,0xff,0xf7,0xff,0xff,0xff,0x7f,0x3f,0xff,0xff,0x00,0xe7,0xf3,0xf9,0xfc,0xfe,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfe,0xfc,0xf9,0xf3,0xc7,0x0f,0x1f,0xe0,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x9f,0x0f,0x3f,0xbf,0x9f,0xff,0xff,0xff,0xff,0xff,0x07,0x7f,0x79,0xfe,0x73,0x6f,0x0f,0x1a,0x84,0x9f,0xbe,0x20,0xd6,0xd8,0xc7,0xe7,0x9e,0x68,0x01,0x39,0x7f,0x7f,0xff,0xff,0xf3,0xfb,0xff,0xf3,0xff,0xff,0xff,0xff,0xcf,0xdf,0xff,0xff,0xff,0xff,0xef,0xf7,0xf7,0xf7,0xef,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x3c,0x24,0xff,0xaf,0xef,0x17,0xc7,0x17,0x53,0x4d,0x69,0xc3,0xff,0x77,0x4f,0x7f,0xbf,0xff,0xff,0xff,0xff,0xff,0xdf,0x0f,0x6e,0x8c,0x5e,0x11,0x23,0x46,0xc4,0xa0,0x30,0x9f,0xcf,0x26,0xc7,0xff,0xf7,0xce,0xbc,0xa0,0x08,0x3b,0x7b,0x77,0x77,0x6f,0x6f,0xdf,0xdf,0xff,0xbf,0xff,0xff,0xff,0xff,0xbf,0xbf,0xbf,0xff,0xff,0xdf,0x9f,0x3f,0x5f,0xdf,0xef,0xeb,0xe3,0xf2,0xf8,0x18,0xfe,0xdf,0x5d,0x77,0xd3,0x9f,0x79,0xf0,0x80,0x64,0x38,0x1b,0x19,0x1b,0x0b,0xcd,0xff,0xff,0xff,0xc7,0x83,0x07,0x07,0x0e,0x0c,0x09,0x0b,0x17,0x17,0x32,0x60,0x00,0x1b,0x04,0x86,0xff,0xff,0x3f,0x37,0x9f,0x9f,0x9e,0x40,0x01,0x0f,0x0f,0x50,0xdf,0xdf,0xcf,0xcf,0xcf,0xcf,0xcf,0x87,0xbb,0xfd,0xfd,0xfe,0x9f,0x9f,0x0f,0x07,0xcf,0xcc,0xc7,0xdf,0x9f,0x9f,0x3f,0x7f,0xfe,0x9d,0x19,0x16,0x24,0x7f,0xe5,0x88,0x10,0x80,0xd0,0x78,0x38,0x3c,0x1e,0x1f,0x0f,0x0f,0x9f,0xff,0xff,0x8f,0x07,0x0e,0x1c,0x3c,0x38,0x88,0x58,0x20,0x20,0x00,0x84,0x84,0x06,0x8f,0x07,0x01,0x3c,0x7c,0xc1,0x9b,0x7b,0xe2,0xf3,0xfb,0xf3,0xf6,0xf7,0xf7,0xf7,0xf7,0x05,0x75,0xf7,0xf7,0xf6,0xf6,0xf6,0xf7,0xf6,0xf0,0xb3,0xf3,0xf3,0xb3,0x01,0x79,0x21,0x18,0xdc,0x00,0x07,0x67,0x66,0x78,0x70,0xe0,0x00,0x81,0xc1,0xc0,0x40,0x40,0x60,0x70,0x30,0x38,0x7c,0x7f,0x7f
};


// ��ʼ��
void oled_init_user() {
   /* OLED_42_Init();
    OLED_92_Init();
//    OLED_42_ShowPicture(0,0,72,42,BMP0,1,1);
    load_show_menu(0);*/
    oled_42_init();
    oled_42_show_pic_index(0,0,72,40, bmp1,1, 0);
    oled_42_show_pic_index(0,0,72,40, bmp2,1, 1);
}

/********************************************************************************
* �˵���ȡ����ʾ
********************************************************************************/
void load_show_menu(uint8_t menu_index) {
    menu_change_lock = 1;
    load_menu_from_flash(menu_index);
    oled_show_pic(menu_index);
    menu_change_lock = 0;
}

/********************************************************************************
* �� flash ����ͼƬ
********************************************************************************/
void load_menu_from_flash(uint8_t folderIndex) {
    for (int i = 0; i < 20; i++) {
        read_from_flash(folderIndex, i, oled_42_array[i], SIZE_42);
        if (i < OLED_96_NUM)
            read_from_flash(folderIndex, i + OLED_42_NUM, oled_96_array[i], SIZE_96);
    }
}

/********************************************************************************
* ��ʾͼƬ
********************************************************************************/
void oled_show_pic(uint8_t menu_index) {
    for (int i = 0; i < OLED_42_NUM; i++) {
        OLED_42_ShowPicture(oled_42_x, oled_42_y, oled_42_l, oled_42_h,
                            oled_42_array[random_refresh[menu_index % 3][i]], 1, random_refresh[menu_index % 3][i]);
        if (i < OLED_96_NUM)
            OLED_92_ShowPicture(oled_96_x, oled_96_y, oled_96_l, oled_96_h, oled_96_array[i], 1, i);
    }
}

/********************************************************************************
* ��Ļ��Ч��ʮ����Ч��
********************************************************************************/
void screen_effect(uint8_t row, uint8_t col, uint8_t mode, uint8_t repeat) {
    if (repeat < 0 || repeat > 10) return;
    uint8_t screen_index = (col * ROW_NUM) + row;
    uint8_t level = 0, left = col - 1, top = row - 1, right = COL_NUM - (COL_NUM - col) + 1, bottom =
            ROW_NUM - (ROW_NUM - row) + 1;
    uint8_t pre = 0;
    OLED_42_ShowPicture(oled_42_x, oled_42_y, oled_42_l, oled_42_h,
                        white_background, 1, screen_index);
    HAL_Delay(30);
    OLED_42_ShowPicture(oled_42_x, oled_42_y, oled_42_l, oled_42_h,
                        oled_42_array[screen_index], 1, screen_index);
    if (pre != 0)
        OLED_42_ShowPicture(oled_42_x, oled_42_y, oled_42_l, oled_42_h,
                            oled_42_array[pre], 1, pre);
    for (; level < 4; level++) {
        if (left >= 0 && left < COL_NUM) {
            screen_index = (left-- * ROW_NUM) + row;
            screen_shake(screen_index, &pre, mode);
        }
        if (top >= 0 && top < ROW_NUM) {
            screen_index = (col * ROW_NUM) + top--;
            screen_shake(screen_index, &pre, mode);
        }
        if (right >= 0 && right < COL_NUM) {
            screen_index = (right++ * ROW_NUM) + row;
            screen_shake(screen_index, &pre, mode);
        }
        if (bottom >= 0 && bottom < ROW_NUM) {
            screen_index = (col * ROW_NUM) + bottom++;
            screen_shake(screen_index, &pre, mode);
        }
    }
    OLED_42_ShowPicture(oled_42_x, oled_42_y, oled_42_l, oled_42_h,
                        oled_42_array[pre], 1, pre);
    if(repeat != 0) {
        HAL_Delay(40);
        screen_effect(row, col, mode, repeat-1);
    }
}

/********************************************************************************
* ��Ļ��Ч�����ˣ�
********************************************************************************/
void screen_effect_two(uint8_t row, uint8_t col, uint8_t mode, uint8_t repeat) {
    if (repeat < 0 || repeat > 10) return;
    uint8_t screen_index = (col * ROW_NUM) + row;
    uint8_t cur_center = (col * ROW_NUM) + row;
    uint8_t level = 0, left = col - 1, top = row - 1, right = COL_NUM - (COL_NUM - col) + 1, bottom =
            ROW_NUM - (ROW_NUM - row) + 1;
    uint8_t pre = 0;
    uint8_t move_block = 0, left_store = 0, top_store = 0, right_store = 0, bottom_store = 0;
    OLED_42_ShowPicture(oled_42_x, oled_42_y, oled_42_l, oled_42_h,
                        white_background, 1, screen_index);
    HAL_Delay(30);
    OLED_42_ShowPicture(oled_42_x, oled_42_y, oled_42_l, oled_42_h,
                        oled_42_array[screen_index], 1, screen_index);
    if (pre != 0)
        OLED_42_ShowPicture(oled_42_x, oled_42_y, oled_42_l, oled_42_h,
                            oled_42_array[pre], 1, pre);
    for (; level < 4; level++) {

        move_block = 222;
        if (left >= 0 && left < COL_NUM) {
            move_block = screen_index = (left-- * ROW_NUM) + row;
            screen_shake(screen_index, &pre, mode);
        }
        // ��
        left_store = move_block;
        for(uint8_t i = 0; i < 4; i ++) {
            if((cur_center - move_block) % 5 != 0 && (move_block > 0 && move_block < 20) && move_block % 4 != 0) {
                if(move_block == 0) break;
                move_block -= 1;
                screen_shake(move_block, &pre, mode);
            }
            if(cur_center - left_store == 12 || cur_center - left_store == 15 || ((cur_center - left_store) % 3 != 0 && (left_store >= 0 && left_store < 20) && (left_store + 1) % 4 != 0)) {
                if(cur_center == 19 || cur_center == 15) continue;
                left_store += 1;
                screen_shake(left_store, &pre, mode);
            }
        }

        move_block = 222;
        if (top >= 0 && top < ROW_NUM) {
            move_block = screen_index = (col * ROW_NUM) + top--;
            screen_shake(screen_index, &pre, mode);
        }
        // ��
        top_store = move_block;
        for(uint8_t i = 0; i < 5; i ++) {
            if(((cur_center - 1) - move_block) % 5 != 0 && (move_block > 0 && move_block < 20)) {
                if(move_block == 0) break;
                move_block -= 4;
                screen_shake(move_block, &pre, mode);
            }
            if((top_store - (cur_center - 1)) % 3 != 0 && (top_store >= 0 && top_store < 20)) {
                top_store += 4;
                screen_shake(top_store, &pre, mode);
            }
        }


        move_block = 222;
        if (right >= 0 && right < COL_NUM) {
            move_block = screen_index = (right++ * ROW_NUM) + row;
            screen_shake(screen_index, &pre, mode);
        }
        // ��
        right_store = move_block;
        for(uint8_t i = 0; i < 4; i ++) {
            if(move_block - cur_center == 12 || move_block - cur_center == 15 || ((move_block - cur_center) % 3 != 0 && (move_block > 0 && move_block < 20) && move_block % 4 != 0)) {
                if(cur_center != 0) {
                    move_block -= 1;
                    screen_shake(move_block, &pre, mode);
                }
            }
            if((right_store - cur_center) % 5 != 0 && (right_store > 0 && right_store < 20) && (right_store + 1) % 4 != 0) {
                if(right_store == 0) break;
                right_store += 1;
                screen_shake(right_store, &pre, mode);
            }
        }

        // ��
        move_block = 222;
        if (bottom >= 0 && bottom < ROW_NUM) {
            move_block = screen_index = (col * ROW_NUM) + bottom++;
            screen_shake(screen_index, &pre, mode);
        }

        bottom_store = move_block;
        for(uint8_t i = 0; i < 5; i ++) {
            if(((cur_center + 1) - move_block) % 3 != 0 && (move_block > 0 && move_block < 20)) {
                if(move_block == 0) break;
                move_block -= 4;
                screen_shake(move_block, &pre, mode);
            }
            if((bottom_store - (cur_center + 1)) % 5 != 0 && (bottom_store > 0 && bottom_store < 20)) {
                if(bottom_store == 0) break;
                bottom_store += 4;
                screen_shake(bottom_store, &pre, mode);
            }
        }
    }
    OLED_42_ShowPicture(oled_42_x, oled_42_y, oled_42_l, oled_42_h,
                        oled_42_array[pre], 1, pre);
    if(repeat != 0) {
        HAL_Delay(40);
        screen_effect_two(row, col, mode, repeat-1);
    }
}

/********************************************************************************
* ��Ļ�л�
********************************************************************************/
static void screen_shake(uint8_t screen_index, uint8_t *pre, uint8_t mode) {
    if(screen_index > 20) return ;
    if (mode == 0) {
        OLED_42_ShowPicture(oled_42_x, oled_42_y, oled_42_l, oled_42_h,
                            white_background, 1, screen_index);
    } else if (mode == 1) {
        OLED_42_ShowPicture(oled_42_x, oled_42_y, oled_42_l, oled_42_h,
                            black_background, 1, screen_index);
    }
    HAL_Delay(5);
    if (pre != 0)
        OLED_42_ShowPicture(oled_42_x, oled_42_y, oled_42_l, oled_42_h,
                            oled_42_array[(*pre)], 1, (*pre));
    (*pre) = screen_index;
}

