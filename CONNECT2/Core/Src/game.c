#include "game.h"
//电脑是1，人类是2
uint8_t checkWin1(uint8_t map[9]) {
    for (int i = 0; i < 3; i++) {
        if (map[i+3*0] == map[i+3*1] && map[i+3*1] == map[i+3*2] && map[i+3*0] != 0) {
            if(mode == 3)
            {
                if(map[i+3*0] == 1)    return 2;
                else if(map[i+3*0] == 2) return 1;
            }
            return map[i+3*0];
        }
    }
    for (int j = 0; j < 3; j++) {
        if (map[0+3*j] == map[1+3*j] && map[1+3*j] == map[2+3*j] && map[0+3*j] != 0) {
            if(mode == 3)
            {
                if(map[0+3*j] == 1)    return 2;
                else if(map[0+3*j] == 2) return 1;
            }  
            return map[0+3*j];          
        }
    }
    if (map[0+3*0] == map[1+3*1] && map[1+3*1] == map[2+3*2] && map[0+3*0] != 0) {
        if(mode == 3)
        {
            if(map[0+3*0] == 1)    return 2;
            else if(map[0+3*0] == 2) return 1;
        }
        return map[0+3*0];
    }
    if (map[0+3*2] == map[1+3*1] && map[1+3*1] == map[2+3*0] && map[0+3*2] != 0) {
        if(mode == 3)
        {
            if(map[0+3*2] == 1)    return 2;
            else if(map[0+3*2] == 2) return 1;
        }
        return map[0+3*2];
    }
    return 0;
}
int checkWin(int map[3][3]) {
    for (int i = 0; i < 3; i++) {
        if (map[i][0] == map[i][1] && map[i][1] == map[i][2] && map[i][0] != 0) {
            return map[i][0];
        }
    }
    for (int j = 0; j < 3; j++) {
        if (map[0][j] == map[1][j] && map[1][j] == map[2][j] && map[0][j] != 0) {
            return map[0][j];
        }
    }
    if (map[0][0] == map[1][1] && map[1][1] == map[2][2] && map[0][0] != 0) {
        return map[0][0];
    }
    if (map[0][2] == map[1][1] && map[1][1] == map[2][0] && map[0][2] != 0) {
        return map[0][2];
    }
    return 0;
}

//map 3*3
int check(int map[3][3]) {
    int Winnum = 8;
    for (int i = 0; i < 3; i++) {//横着
        for (int j = 0; j < 3; j++) {
            if (map[i][j] == 2) {
                Winnum--;
                break;
            }

        }
    }
    for (int i = 0; i < 3; i++) {//竖着
        for (int j = 0; j < 3; j++) {
            if (map[j][i] == 2) {
                Winnum--;
                break;
            }

        }
    }
    for (int i = 0; i < 3; i++) {
        if (map[i][i] == 2) {
            Winnum--;
            break;
        }
    }
    for (int i = 0; i < 3; i++) {
        if (map[i][2 - i] == 2) {
            Winnum--;
            break;
        }
    }
    return Winnum;
}

int player(int map[3][3]) {
    int Winnum = 9;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (map[i][j] == 0) {
                map[i][j] = 2;
                //玩家要使Winnum最小
                int temp = check(map);
                if (Winnum > temp) {
                    Winnum = temp;

                }
                map[i][j] = 0;
            }
        }
    }
    return Winnum;
}

void game(uint8_t map1[9],int *best_position) {
    int Winnum = -1;
    int map[3][3]={0};
    
    if(map1[0]==1&&map1[1]==2&&map1[2]==0&&map1[3]==0&&map1[4]==1&&map1[5]==0&&map1[6]==0&&map1[7]==0&&map1[8]==2){
        *(best_position) = 1;
        *(best_position+1) = 0;
        return ;
    }
			

	for(int i=0;i<3;++i)
	{
		for(int j=0;j<3;j++)
		{
            if(mode == 3&&mode == 5)
            {
                if(map1[i*3+j] == 1)    map[i][j] = 2;
                else if(map1[i*3+j] == 2) map[i][j] = 1;
            }
            else
			map[i][j] = map1[i*3+j];
		}
	}

	    if(map[0][0] == 2&&map[0][1] == 1&&map[0][2]==0&&map[1][0]==0&&map[1][1]==1&&map[1][2]==0&&map[2][0]==0&&map[2][1]==2&&map[2][2]==0)
				{
        *(best_position) = 2;
        *(best_position+1) = 0;
        return ;
    }
    for (int i = 0; i < 9; i++) {
        if (map[i / 3][i % 3] == 0) {
            map[i / 3][i % 3] = 1;
            switch (checkWin(map)) {//电脑win
                case 1://电脑win
                    map[i / 3][i % 3] = 0;
                     *(best_position) = i / 3;
                     *(best_position+1) = i % 3;
                    
										return;
            }

            map[i / 3][i % 3] = 2;
            switch (checkWin(map)) {//电脑win
                case 2://电脑win
                    map[i / 3][i % 3] = 0;
                     *(best_position) = i / 3;
                     *(best_position+1) = i % 3;
                    return ;
            }
            map[i / 3][i % 3] = 0;
        }
    }
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (map[i][j] == 0) {
                map[i][j] = 1;
                //电脑要使Winnum最大
                int temp = player(map);
                if (Winnum < temp) {
                    Winnum = temp;
                    *(best_position) = i;
                    *(best_position+1) = j;
                }

                map[i][j] = 0;
            }
        }
    }
    return ;

}

