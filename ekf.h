/*
 * ekf.h
 *
 *  Created on: Apr 25, 2019
 *      Author: UJJWAL
 */

#ifndef EKF_H_
#define EKF_H_

#define dim_Q 5
#define dim_R 2

#define TOL 1
#define TWO_PI 6.28318530718

typedef float data_t;

void ekf(data_t x_in[dim_Q], data_t u_in[dim_R], data_t y_in[dim_R], data_t x_out[dim_Q], data_t p[dim_Q][dim_Q]);

#endif /* EKF_H_ */
