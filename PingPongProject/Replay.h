/*
 * Replay.h
 *
 * Created: 12.11.2021 16:59:53
 *  Author: harishnv
 */ 


#ifndef REPLAY_H_
#define REPLAY_H_

void replay_store(CAN_Msg msg);
void replay_retrieve();

#endif /* REPLAY_H_ */