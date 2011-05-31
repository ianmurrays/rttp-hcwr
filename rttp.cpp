/*
 *  rttp.cpp
 *  rttp-xcode
 *
 *  Created by Ian Murray on 31-05-11.
 *  Copyright 2011 Linkworks. All rights reserved.
 *
 */

#include "rttp.h"

RTTP::RTTP(int numberOfTeams, int numberOfDays, int maxConsecutiveOffDays, int maxConsecutiveGames)
{
  this.numberOfTeams = numberOfTeams;
  this.numberOfDays = numberOfDays;
  this.maxConsecutiveOffDays = maxConsecutiveOffDays;
  this.maxConsecutiveGames = maxConsecutiveGames;
}

RTTP::~RTTP()
{
  // TODO: Free up memory?
}

RTTP::noConsecutiveHomeGames()
{
  
}