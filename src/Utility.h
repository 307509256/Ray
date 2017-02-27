#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <ctime>

// ����Ԃ̎Z�o
double GetSpendTime( clock_t start_time );

#if !defined (_WIN32)
double GetSpendTimeForLinux( struct timeval *start_time );
#endif

// �f�[�^�ǂݍ���(float)
void InputTxtFLT( const char *filename, float *ap, int array_size );

// �f�[�^�ǂݍ���(double)
void InputTxtDBL( const char *filename, double *ap, int array_size );

#endif
