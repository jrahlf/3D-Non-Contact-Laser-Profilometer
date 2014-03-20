
/*
* Copyright (c) 2010, Arizona Robotics Research Group, University of Arizona
* Copyright (c) 2008, Willow Garage, Inc.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* * Redistributions of source code must retain the above copyright
* notice, this list of conditions and the following disclaimer.
* * Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in the
* documentation and/or other materials provided with the distribution.
* * Neither the name of the <ORGANIZATION> nor the names of its
* contributors may be used to endorse or promote products derived from
* this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTA        L, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*/

#include <termios.h>
#include <signal.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/poll.h>
#include <pthread.h>
#include <signal.h>
#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <string>

#define KEYCODE_W 0x77
#define KEYCODE_A 0x61
#define KEYCODE_S 0x73
#define KEYCODE_D 0x64

#define KEYCODE_A_CAP 0x41
#define KEYCODE_D_CAP 0x44
#define KEYCODE_S_CAP 0x53
#define KEYCODE_W_CAP 0x57

#define KEYCODE_A_CTRL 0x01
#define KEYCODE_D_CTRL 0x04
#define KEYCODE_S_CTRL 0x13
#define KEYCODE_W_CTRL 0x17

#define KEYCODE_SPACE 0x20
#define KEYCODE_ENTER 0x0d

using namespace std;

double VELOCITY_VERY_SLOW = 0.1;
double VELOCITY_SLOW = 0.4;
double VELOCITY = 1;
double samplingResolution = -10;
ofstream serial;

int kfd = 0;
struct termios cooked, raw;

void keyboardLoop();

void siginthandler(int param){
    tcsetattr(kfd, TCSANOW, &cooked);
    exit(1);
}


int main(int argc, char** argv){
    signal(SIGINT, siginthandler);
    serial.open("/dev/ttyUSB0", ios::binary);
    if(argc > 1){
        samplingResolution = strtod(argv[1], 0);
        cout << "Setting sampling resolution to " << samplingResolution << endl;
    }
    if(argc > 2){
        double maxSpeed = strtod(argv[2], 0);
        cout << "Setting maximum speed to " << maxSpeed << endl;
        serial << "MAXS," << maxSpeed << endl;
    }
    if(samplingResolution > 0){
        serial << "SETT," << samplingResolution << endl;
    }
    //int fd = open("/dev/ttyUSB0");
    keyboardLoop();
  return(0);
}

double oldx = -2;
double oldy = -2;

void setSpeedX(double speed){
    if(speed == oldx){
        return;
    }
    oldx = speed;
    //cout << " x " << speed << endl;
    serial << "SPEX," << speed << endl;
}

void setSpeedY(double speed){
    if(speed == oldy){
        return;
    }
    oldy = speed;
    serial << "SPEY," << speed << endl;
}

void placeLinePoint(){
    serial << "PAL1" << endl;
}

void placePoint(){
    serial << "PAR3,10" << endl;
}

void keyboardLoop(){
  char c;
  double speedX;
  double speedY;
  int dirty = 0;

  // get the console in raw mode
  tcgetattr(kfd, &cooked);
  memcpy(&raw, &cooked, sizeof(struct termios));
  raw.c_iflag |= IGNBRK;
  raw.c_lflag &=~ (ICANON | ECHO);
  raw.c_iflag &= ~(INLCR | ICRNL | IXON | IXOFF);
  raw.c_cc[VEOL] = 1;
  raw.c_cc[VEOF] = 2;
  tcsetattr(kfd, TCSANOW, &raw);

  puts("Reading from keyboard");
  puts("Use W/S for y axis");
  puts("Use A/D for x axis");
  puts("Hold Shift to move slower");
  puts("Hold Ctrl to move extremely slow");
  puts("Press space to place a point for a rectangle pattern");
  puts("Press enter to place a point for a line");
  struct pollfd ufd;
  ufd.fd = kfd;
  ufd.events = POLLIN;

  for(;;){

    // get the next event from the keyboard
    int num;

    if((num = poll(&ufd, 1, 200)) < 0){
      perror("poll():");
      return;
    }
    else if(num > 0){
      if(read(kfd, &c, 1) < 0){
        perror("read():");
        return;
      }
    }
    else{
      if (dirty >= 2){
          //cout << "dirty " << endl;
          //set speed to 0
          setSpeedX(0);
          setSpeedY(0);
          dirty = 0;
      }else{
          dirty++;
      }
      continue;
    }

    switch(c){
        case KEYCODE_W:
         speedX = VELOCITY;
         speedY = 0;
         dirty++;
         break;
        case KEYCODE_S:
        speedX = -VELOCITY;
        speedY = 0;
         dirty++;
         break;
        case KEYCODE_A:
        speedX = 0;
        speedY = -VELOCITY;
        dirty++;
         break;
        case KEYCODE_D:
        speedX = 0;
        speedY = VELOCITY;
         dirty++;
         break;

        case KEYCODE_W_CAP:
        speedX = VELOCITY_SLOW;
        speedY = 0;
         dirty++;
         break;
        case KEYCODE_S_CAP:
         speedX = -VELOCITY_SLOW;
         speedY = 0;
         dirty++;
         break;
        case KEYCODE_A_CAP:
        speedX = 0;
        speedY = -VELOCITY_SLOW;
         dirty++;
         break;
        case KEYCODE_D_CAP:
        speedX = 0;
        speedY = VELOCITY_SLOW;
         dirty++;
         break;
         
        case KEYCODE_W_CTRL:
        speedX = VELOCITY_VERY_SLOW;
        speedY = 0;
         dirty++;
         break;
        case KEYCODE_S_CTRL:
         speedX = -VELOCITY_VERY_SLOW;
         speedY = 0;
         dirty++;
         break;
        case KEYCODE_A_CTRL:
        speedX = 0;
        speedY = -VELOCITY_VERY_SLOW;
         dirty++;
         break;
        case KEYCODE_D_CTRL:
        speedX = 0;
        speedY = VELOCITY_VERY_SLOW;
         dirty++;
         break;

    case KEYCODE_SPACE:
        placePoint();
        speedX = 0;
        speedY = 0;
        break;

    case KEYCODE_ENTER:
        placeLinePoint();
        speedX = 0;
        speedY = 0;
        break;
         
    default:
        speedX = 0;
        speedY = 0;
        dirty++;
    }

    setSpeedX(speedY);
    setSpeedY(speedX);
  }
}
