//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////_____________/////_____//////____/////_____________/////____/////____/////____________//////////////
//////////////____//////////////______/////____/////____/////____/////____////____//////____//////////////////////
//////////////____//////////////_______////____/////____/////____/////____///____///////____//////////////////////
//////////////____//////////////____/___///____/////____/////____/////____//____////////____//////////////////////
//////////////_____________/////____//___//____/////_____________/////_________/////////__________////////////////
///////////////////////____/////____///___/____/////____/////____/////____//____////////____//////////////////////
///////////////////////____/////____////_______/////____/////____/////____///____///////____//////////////////////
///////////////////////____/////____/////______/////____/////____/////____////____//////____//////////////////////
//////////////_____________/////____//////_____/////____/////____/////____/////____/////____________//////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include <stdbool.h>  /* Declarations for bool */


int snake_direction_x = 1;
int snake_direction_y = 0;

int snake [200][3]; //200 possible possitions. 3 parts, part-0 = x, part-1 = y, part-2 = if tile exists.
int food [2]; //0 = x, 1 = y.
int score = 0;

void snake_plus_length ()
{
    int i;
    int snake_end = 0;
    for (i = 0; snake[i][2] == 1 && i <200; i++)
    {
       snake_end++;
    }
	for ( i = 3; i < 200; i++) // starts at 3 becaus length of snake is initally 3
	{
		if (snake [i][2] == 0)
		{
			snake[i][2] = 1;
			snake[i][1] = snake [i-1][1];
            snake[i][0] = snake [i-1][0];
            for (i = snake_end; i > 0; i--) 
            {
                snake[i][0] =  snake[i-1][0];
                snake[i][1] =  snake[i-1][1];
            }
            snake[0][0] = snake[0][0] + snake_direction_x;
            snake[0][1] = snake[0][1] + snake_direction_y; 
			return;
		}
	}
}

void next_tile ()
{
    int snake_end = 0;
    int i;
    for (i = 0; snake[i][2] == 1 && i <200; i++)
    {
       snake_end++;
    }
    for (i = snake_end; i > 0; i--) 
    {
        snake[i][0] =  snake[i-1][0];
        snake[i][1] =  snake[i-1][1];
    }
    snake[0][0] = snake[0][0] + snake_direction_x;
    snake[0][1] = snake[0][1] + snake_direction_y;    
}

void snake_to_display()
{
    int y, x, i, j, h;
    for ( i = 0; snake[i][2] == 1 && i < 200; i++)
    {
        y = (snake[i][1] * 4);
        x = (snake[i][0] * 4);
        for(j = 0; j < 4; j++)
        {
            for(h = 0; h < 4; h++)
            {
                display[y + j][x + h] = 1;
            }
        }
    }
}
void food_to_display()
{
    int y, x;
    x = (food[0]* 4);
    y = (food[1] * 4);
    display[y][x + 1] = 1;
    display[y][x + 2] = 1;

    display[y + 1][x] = 1;
    display[y + 1][x + 3] = 1;

    display[y + 2][x] = 1;
    display[y + 2][x + 3] = 1;

    display[y + 3][x + 1] = 1;
    display[y + 3][x + 2] = 1;
}

void check_if_turn_then_turn (bool button_left, bool button_right)
{
    if (button_left)
    {
        if (snake_direction_y == 0)
        {
            if (snake_direction_x == 1)
            {
                snake_direction_x = 0;
                snake_direction_y = -1;
            }
            else //if (snake_direction_x == -1)
            {
                snake_direction_x = 0;
                snake_direction_y = 1;
            }
        }
        else //if (snake_direction_x == 0)
        {
            if (snake_direction_y == 1)
            {
                snake_direction_x = 1 ;
                snake_direction_y = 0;
            }
            else //if (snake_direction_y == -1)
            {
                snake_direction_x = -1;
                snake_direction_y = 0;
            }
        }
        
    }
    else if (button_right)
    {
        if (snake_direction_y == 0)
        {
            if (snake_direction_x == 1)
            {
                snake_direction_x = 0;
                snake_direction_y = 1;
            }
            else //if (snake_direction_x == -1)
            {
                snake_direction_x = 0;
                snake_direction_y = -1;
            }
        }
        else //if (snake_direction_x == 0)
        {
            if (snake_direction_y == 1)
            {
                snake_direction_x = -1;
                snake_direction_y = 0;
            }
            else //if (snake_direction_y == -1)
            {
                snake_direction_x = 1;
                snake_direction_y = 0;
            }
        }
    }
}

bool check_gameover()
{
    // if function returns 0 means game over
    // if function returns 1 keep play
    int i;
    for (i = 1; snake[i][2] == 1 && i < 200; i++)
    {
        if ((snake[0][0] == snake[i][0] && snake[0][1] == snake[i][1]) || (snake[0][0] < 0 || snake[0][0] > 24 || snake[0][1] < 0 || snake[0][1] > 7))
        {
            return 0;
        }
    }
    return 1;
}

bool snake_eat_food (int i)
{
    if(snake[i][0] == food [0] && snake[i][1] == food [1])
    {
        return 1;
    }
    return 0;
}

void make_food()
{
    int temp = 1;
    food[0] = food_x[score%10]; 
    food[1] = food_y[score%8];  
    int i;
    for (i = 0; i < 200 && snake[i][2] == 1; i++)
    {
        if(snake_eat_food(i))
        {
            i = 0;
            food [0] = food_x[(score + temp) %10];
            food[1] = food_y[(score + temp + temp) %8];
            temp++;
        }
    }
}

bool check_left_button(int buttons)
{
    if(buttons == 4)
    {
        return 1;
    }
    return 0;
}

bool check_right_button(int buttons)
{
    if(buttons == 2)
    {
        return 1;
    }
    return 0;
}

void print_gameover()
{
    //E
       display[14][45] = 1;
    display[15][45] = 1;
    display[16][45] = 1;
    display[17][45] = 1;
    display[18][45] = 1;

    display[14][46] = 1;
    display[15][46] = 0;
    display[16][46] = 1;
    display[17][46] = 0;
    display[18][46] = 1;

    display[14][47] = 1;
    display[15][47] = 0;
    display[16][47] = 1;
    display[17][47] = 0;
    display[18][47] = 1;

    display[14][48] = 1;
    display[15][48] = 0;
    display[16][48] = 1;
    display[17][48] = 0;
    display[18][48] = 1;

    display[14][49] = 1;
    display[15][49] = 0;
    display[16][49] = 0;
    display[17][49] = 0;
    display[18][49] = 1;

    //N
    display[14][51] = 1;
    display[15][51] = 1;
    display[16][51] = 1;
    display[17][51] = 1;
    display[18][51] = 1;

    display[14][52] = 0;
    display[15][52] = 1;
    display[16][52] = 0;
    display[17][52] = 0;
    display[18][52] = 0;

    display[14][53] = 0;
    display[15][53] = 0;
    display[16][53] = 1;
    display[17][53] = 0;
    display[18][53] = 0;

    display[14][54] = 0;
    display[15][54] = 0;
    display[16][54] = 0;
    display[17][54] = 1;
    display[18][54] = 0;

    display[14][55] = 1;
    display[15][55] = 1;
    display[16][55] = 1;
    display[17][55] = 1;
    display[18][55] = 1;

    //D
    display[14][57] = 1;
    display[15][57] = 1;
    display[16][57] = 1;
    display[17][57] = 1;
    display[18][57] = 1;

    display[14][58] = 1;
    display[15][58] = 0;
    display[16][58] = 0;
    display[17][58] = 0;
    display[18][58] = 1;

    display[14][59] = 1;
    display[15][59] = 0;
    display[16][59] = 0;
    display[17][59] = 0;
    display[18][59] = 1;

    display[14][60] = 1;
    display[15][60] = 0;
    display[16][60] = 0;
    display[17][60] = 0;
    display[18][60] = 1;

    display[14][61] = 0;
    display[15][61] = 1;
    display[16][61] = 1;
    display[17][61] = 1;
    display[18][61] = 0;
}

void print_win()
{
    //W
    display[14][45] = 1;
    display[15][45] = 1;
    display[16][45] = 0;
    display[17][45] = 0;
    display[18][45] = 0;

    display[14][46] = 0;
    display[15][46] = 0;
    display[16][46] = 1;
    display[17][46] = 1;
    display[18][46] = 1;

    display[14][47] = 0;
    display[15][47] = 0;
    display[16][47] = 0;
    display[17][47] = 1;
    display[18][47] = 0;

    display[14][48] = 0;
    display[15][48] = 0;
    display[16][48] = 1;
    display[17][48] = 1;
    display[18][48] = 1;

    display[14][49] = 1;
    display[15][49] = 1;
    display[16][49] = 0;
    display[17][49] = 0;
    display[18][49] = 0;

    //I
    display[14][51] = 0;
    display[15][51] = 0;
    display[16][51] = 0;
    display[17][51] = 0;
    display[18][51] = 0;

    display[14][52] = 1;
    display[15][52] = 0;
    display[16][52] = 0;
    display[17][52] = 0;
    display[18][52] = 1;

    display[14][53] = 1;
    display[15][53] = 1;
    display[16][53] = 1;
    display[17][53] = 1;
    display[18][53] = 1;

    display[14][54] = 1;
    display[15][54] = 0;
    display[16][54] = 0;
    display[17][54] = 0;
    display[18][54] = 1;

    display[14][55] = 0;
    display[15][55] = 0;
    display[16][55] = 0;
    display[17][55] = 0;
    display[18][55] = 0;

    //N
    display[14][57] = 1;
    display[15][57] = 1;
    display[16][57] = 1;
    display[17][57] = 1;
    display[18][57] = 1;

    display[14][58] = 0;
    display[15][58] = 1;
    display[16][58] = 0;
    display[17][58] = 0;
    display[18][58] = 0;

    display[14][59] = 0;
    display[15][59] = 0;
    display[16][59] = 1;
    display[17][59] = 0;
    display[18][59] = 0;

    display[14][60] = 0;
    display[15][60] = 0;
    display[16][60] = 0;
    display[17][60] = 1;
    display[18][60] = 0;

    display[14][61] = 1;
    display[15][61] = 1;
    display[16][61] = 1;
    display[17][61] = 1;
    display[18][61] = 1;
}

void snake_init()
{
    int i;
    snake[0][0] = 13;
    snake[0][1] = 4;
    snake[1][0] = 12;
    snake[1][1] = 4;
    snake[2][0] = 11;
    snake[3][1] = 4;
    snake[0][2] = 1;
    snake[1][2] = 1;
    snake[2][2] = 1;
    for(i = 3; snake[i][2] == 1 && i < 200; i++)
    {
        snake[i][0] = 0;
        snake[i][1] = 0;
        snake[i][2] = 0;
    }
    score = 0;
    snake_direction_x = 1;
    snake_direction_y = 0;
    make_food();
    food_to_display();
    snake_to_display();
}

int snake_run()// run all
{
    if(score >= 25 && check_gameover())
    {
        print_win();
    }
    else if (check_gameover())
    {    
        int buttons = getbtns();
        check_if_turn_then_turn (check_left_button(buttons), check_right_button(buttons));
        if (snake_eat_food(0))
        {
            snake_plus_length();
            score++;
            make_food(); 
        }
        else
        {
            next_tile ();
        }
        snake_to_display();
        food_to_display();
        delay( 500 );     
    }
    else
    {
        print_gameover();
       
    }
    return score;
}