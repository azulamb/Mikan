#ifndef K_ESCAPE
#include <dinput.h>
// ÉLÅ[íËêî
#define K_ESCAPE       DIK_ESCAPE       /* 0x01 : Escape */
#define K_1            DIK_1            /* 0x02 : 1 */
#define K_2            DIK_2            /* 0x03 : 2 */
#define K_3            DIK_3            /* 0x04 : 3 */
#define K_4            DIK_4            /* 0x05 : 4 */
#define K_5            DIK_5            /* 0x06 : 5 */
#define K_6            DIK_6            /* 0x07 : 6 */
#define K_7            DIK_7            /* 0x08 : 7 */
#define K_8            DIK_8            /* 0x09 : 8 */
#define K_9            DIK_9            /* 0x0A : 9 */
#define K_0            DIK_0            /* 0x0B : 0 */
#define K_MINUS        DIK_MINUS        /* 0x0C : - on main keyboard */
#define K_EQUALS       DIK_EQUALS       /* 0x0D : = */
#define K_BACK         DIK_BACK         /* 0x0E : backspace */
#define K_TAB          DIK_TAB          /* 0x0F : Tab */
#define K_Q            DIK_Q            /* 0x10 : Q */
#define K_W            DIK_W            /* 0x11 : W */
#define K_E            DIK_E            /* 0x12 : E */
#define K_R            DIK_R            /* 0x13 : R */
#define K_T            DIK_T            /* 0x14 : T */
#define K_Y            DIK_Y            /* 0x15 : Y */
#define K_U            DIK_U            /* 0x16 : U */
#define K_I            DIK_I            /* 0x17 : I */
#define K_O            DIK_O            /* 0x18 : O */
#define K_P            DIK_P            /* 0x19 : P */
#define K_LBRACKET     DIK_LBRACKET     /* 0x1A : ( */
#define K_RBRACKET     DIK_RBRACKET     /* 0x1B : ) */
#define K_RETURN       DIK_RETURN       /* 0x1C : Enter on main keyboard */
#define K_LCONTROL     DIK_LCONTROL     /* 0x1D : Left Control key*/
#define K_A            DIK_A            /* 0x1E : A */
#define K_S            DIK_S            /* 0x1F : S */
#define K_D            DIK_D            /* 0x20 : D */
#define K_F            DIK_F            /* 0x21 : F */
#define K_G            DIK_G            /* 0x22 : G */
#define K_H            DIK_H            /* 0x23 : H */
#define K_J            DIK_J            /* 0x24 : J */
#define K_K            DIK_K            /* 0x25 : K */
#define K_L            DIK_L            /* 0x26 : L */
#define K_SEMICOLON    DIK_SEMICOLON    /* 0x27 : ; */
#define K_APOSTROPHE   DIK_APOSTROPHE   /* 0x28 : ` */
#define K_GRAVE        DIK_GRAVE        /* 0x29 : accent grave */
#define K_LSHIFT       DIK_LSHIFT       /* 0x2A : Left Shift key*/
#define K_BACKSLASH    DIK_BACKSLASH    /* 0x2B : \ */
#define K_Z            DIK_Z            /* 0x2C : Z */
#define K_X            DIK_X            /* 0x2D : X */
#define K_C            DIK_C            /* 0x2E : C */
#define K_V            DIK_V            /* 0x2F : V */
#define K_B            DIK_B            /* 0x30 : B */
#define K_N            DIK_N            /* 0x31 : N */
#define K_M            DIK_M            /* 0x32 : M */
#define K_COMMA        DIK_COMMA        /* 0x33 : , */
#define K_PERIOD       DIK_PERIOD       /* 0x34 : . on main keyboard */
#define K_SLASH        DIK_SLASH        /* 0x35 : / on main keyboard */
#define K_RSHIFT       DIK_RSHIFT       /* 0x36 : Right Shift key */
#define K_MULTIPLY     DIK_MULTIPLY     /* 0x37 : * on numeric keypad */
#define K_LMENU        DIK_LMENU        /* 0x38 : left Alt */
#define K_SPACE        DIK_SPACE        /* 0x39 : Space */
#define K_CAPITAL      DIK_CAPITAL      /* 0x3A :  */
#define K_F1           DIK_F1           /* 0x3B : Function 1 */
#define K_F2           DIK_F2           /* 0x3C : Function 2 */
#define K_F3           DIK_F3           /* 0x3D : Function 3 */
#define K_F4           DIK_F4           /* 0x3E : Function 4 */
#define K_F5           DIK_F5           /* 0x3F : Function 5 */
#define K_F6           DIK_F6           /* 0x40 : Function 6 */
#define K_F7           DIK_F7           /* 0x41 : Function 7 */
#define K_F8           DIK_F8           /* 0x42 : Function 8 */
#define K_F9           DIK_F9           /* 0x43 : Function 9 */
#define K_F10          DIK_F10          /* 0x44 : Function 10 */
#define K_NUMLOCK      DIK_NUMLOCK      /* 0x45 : Numlock */
#define K_SCROLL       DIK_SCROLL       /* 0x46 : Scroll Lock */
#define K_NUMPAD7      DIK_NUMPAD7      /* 0x47 : 7 on numeric keyboard */
#define K_NUMPAD8      DIK_NUMPAD8      /* 0x48 : 8 on numeric keyboard */
#define K_NUMPAD9      DIK_NUMPAD9      /* 0x49 : 9 on numeric keyboard */
#define K_SUBTRACT     DIK_SUBTRACT     /* 0x4A : - on numeric keypad */
#define K_NUMPAD4      DIK_NUMPAD4      /* 0x4B : 4 on numeric keyboard */
#define K_NUMPAD5      DIK_NUMPAD5      /* 0x4C : 5 on numeric keyboard */
#define K_NUMPAD6      DIK_NUMPAD6      /* 0x4D : 6 on numeric keyboard */
#define K_ADD          DIK_ADD          /* 0x4E : + on numeric keypad */
#define K_NUMPAD1      DIK_NUMPAD1      /* 0x4F : 1 on numeric keyboard */
#define K_NUMPAD2      DIK_NUMPAD2      /* 0x50 : 2 on numeric keyboard */
#define K_NUMPAD3      DIK_NUMPAD3      /* 0x51 : 3 on numeric keyboard */
#define K_NUMPAD0      DIK_NUMPAD0      /* 0x52 : 0 on numeric keyboard */
#define K_DECIMAL      DIK_DECIMAL      /* 0x53 : . on numeric keypad */
#define K_OEM_102      DIK_OEM_102      /* 0x54 : <> or \| on RT 102-key keyboard (Non-U.S.) */
#define K_F11          DIK_F11          /* 0x55 : Function 11 */
#define K_F12          DIK_F12          /* 0x56 : Function 12 */
#define K_F13          DIK_F13          /* 0x64 : (NEC PC98) */
#define K_F14          DIK_F14          /* 0x65 : (NEC PC98) */
#define K_F15          DIK_F15          /* 0x66 : (NEC PC98) */
#define K_KANA         DIK_KANA         /* 0x70 : (Japanese keyboard)            */
#define K_ABNT_C1      DIK_ABNT_C1      /* 0x73 : /? on Brazilian keyboard */
#define K_CONVERT      DIK_CONVERT      /* 0x79 : (Japanese keyboard)            */
#define K_NOCONVERT    DIK_NOCONVERT    /* 0x7B : (Japanese keyboard)            */
#define K_YEN          DIK_YEN          /* 0x7D : (Japanese keyboard)            */
#define K_ABNT_C2      DIK_ABNT_C2      /* 0x7E : Numpad . on Brazilian keyboard */
#define K_NUMPADEQUALS DIK_NUMPADEQUALS /* 0x8D : = on numeric keypad (NEC PC98) */
#define K_PREVTRACK    DIK_PREVTRACK    /* 0x90 : Previous Track (DIK_CIRCUMFLEX on Japanese keyboard) */
#define K_AT           DIK_AT           /* 0x91 : (NEC PC98) */
#define K_COLON        DIK_COLON        /* 0x92 : (NEC PC98) */
#define K_UNDERLINE    DIK_UNDERLINE    /* 0x93 : (NEC PC98) */
#define K_KANJI        DIK_KANJI        /* 0x94 : (Japanese keyboard)            */
#define K_STOP         DIK_STOP         /* 0x95 : (NEC PC98) */
#define K_AX           DIK_AX           /* 0x96 : (Japan AX) */
#define K_UNLABELED    DIK_UNLABELED    /* 0x97 : (J3100) */
#define K_NEXTTRACK    DIK_NEXTTRACK    /* 0x99 : Next Track */
#define K_NUMPADENTER  DIK_NUMPADENTER  /* 0x9C : Enter on numeric keypad */
#define K_RCONTROL     DIK_RCONTROL     /* 0x9D : Right Controll key */
#define K_MUTE         DIK_MUTE         /* 0xA0 : Mute */
#define K_CALCULATOR   DIK_CALCULATOR   /* 0xA1 : Calculator */
#define K_PLAYPAUSE    DIK_PLAYPAUSE    /* 0xA2 : Play / Pause */
#define K_MEDIASTOP    DIK_MEDIASTOP    /* 0xA4 : Media Stop */
#define K_VOLUMEDOWN   DIK_VOLUMEDOWN   /* 0xAE : Volume - */
#define K_VOLUMEUP     DIK_VOLUMEUP     /* 0xB0 : Volume + */
#define K_WEBHOME      DIK_WEBHOME      /* 0xB2 : Web home */
#define K_NUMPADCOMMA  DIK_NUMPADCOMMA  /* 0xB3 : , on numeric keypad (NEC PC98) */
#define K_DIVIDE       DIK_DIVIDE       /* 0xB5 : / on numeric keypad */
#define K_SYSRQ        DIK_SYSRQ        /* 0xB7 :  */
#define K_RMENU        DIK_RMENU        /* 0xB8 : right Alt */
#define K_PAUSE        DIK_PAUSE        /* 0xC5 : Pause */
#define K_HOME         DIK_HOME         /* 0xC7 : Home on arrow keypad */
#define K_UP           DIK_UP           /* 0xC8 : UpArrow on arrow keypad */
#define K_PRIOR        DIK_PRIOR        /* 0xC9 : PgUp on arrow keypad */
#define K_LEFT         DIK_LEFT         /* 0xCB : LeftArrow on arrow keypad */
#define K_RIGHT        DIK_RIGHT        /* 0xCD : RightArrow on arrow keypad */
#define K_END          DIK_END          /* 0xCF : End on arrow keypad */
#define K_DOWN         DIK_DOWN         /* 0xD0 : DownArrow on arrow keypad */
#define K_NEXT         DIK_NEXT         /* 0xD1 : PgDn on arrow keypad */
#define K_INSERT       DIK_INSERT       /* 0xD2 : Insert on arrow keypad */
#define K_DELETE       DIK_DELETE       /* 0xD3 : Delete on arrow keypad */
#define K_LWIN         DIK_LWIN         /* 0xDB : Left Windows key */
#define K_RWIN         DIK_RWIN         /* 0xDC : Right Windows key */
#define K_APPS         DIK_APPS         /* 0xDD : AppMenu key */
#define K_POWER        DIK_POWER        /* 0xDE : System Power */
#define K_SLEEP        DIK_SLEEP        /* 0xDF : System Sleep */
#define K_WAKE         DIK_WAKE         /* 0xE3 : System Wake */
#define K_WEBSEARCH    DIK_WEBSEARCH    /* 0xE5 : Web Search */
#define K_WEBFAVORITES DIK_WEBFAVORITES /* 0xE6 : Web Favorites */
#define K_WEBREFRESH   DIK_WEBREFRESH   /* 0xE7 : Web Refresh */
#define K_WEBSTOP      DIK_WEBSTOP      /* 0xE8 : Web Stop */
#define K_WEBFORWARD   DIK_WEBFORWARD   /* 0xE9 : Web Forward */
#define K_WEBBACK      DIK_WEBBACK      /* 0xEA : Web Back */
#define K_MYCOMPUTER   DIK_MYCOMPUTER   /* 0xEB : My Computer */
#define K_MAIL         DIK_MAIL         /* 0xEC : Mail */
#define K_MEDIASELECT  DIK_MEDIASELECT  /* 0xED : Media Select */

/*
 *  Alternate names for keys, to facilitate transition from DOS.
 */
#define K_ESC          K_ESCAPE         /* 0x01 : Escape */
#define K_ENTER        K_RETURN         /* Enter */
#define K_BACKSPACE    K_BACK           /* backspace */
#define K_NUMPADSTAR   K_MULTIPLY       /* * on numeric keypad */
#define K_LALT         K_LMENU          /* left Alt */
#define K_CAPSLOCK     K_CAPITAL        /* CapsLock */
#define K_NUMPADMINUS  K_SUBTRACT       /* - on numeric keypad */
#define K_NUMPADPLUS   K_ADD            /* + on numeric keypad */
#define K_NUMPADPERIOD K_DECIMAL        /* . on numeric keypad */
#define K_NUMPADSLASH  K_DIVIDE         /* / on numeric keypad */
#define K_RALT         K_RMENU          /* right Alt */
#define K_UPARROW      K_UP             /* UpArrow on arrow keypad */
#define K_PGUP         K_PRIOR          /* PgUp on arrow keypad */
#define K_LEFTARROW    K_LEFT           /* LeftArrow on arrow keypad */
#define K_RIGHTARROW   K_RIGHT          /* RightArrow on arrow keypad */
#define K_DOWNARROW    K_DOWN           /* DownArrow on arrow keypad */
#define K_PGDN         K_NEXT           /* PgDn on arrow keypad */

#endif
