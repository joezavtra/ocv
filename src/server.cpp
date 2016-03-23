/*
 * server.cpp
 *
 *  Created on: 31 янв. 2016 г.
 *      Author: pavelzelenov
 */

#include <stdio.h>
#include <iostream>
#include <string>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/cvstd.hpp"

#include "tracker.hpp"
#include "monitor.hpp"

using namespace std;


int main(int argc, char** argv)
{
	 using namespace std::chrono;

	monitor *m;

	for (int i = 1; i < argc; ++i) {
		std::string an = argv[i];
		if ((an == "-d") || (an == "--device")) {

			std::string av = argv[++i];
			std::cout << "Open device: " << av << "\n";
			m = new monitor (std::stoi(av));

		} else if ((an == "-f") || (an == "--file")) {

			std::string av = argv[++i];
			std::cout << "Open file: " << av << "\n";
			m = new monitor (av.c_str());
		}
	}

	if( !*m ) m = new monitor( 0 );

	while( m->players_count() == 0 )
	{
		m->find();
		m->show();
	}
	while(1) {
		auto start = std::chrono::high_resolution_clock::now();
		m->getFrame();
		auto frame = std::chrono::high_resolution_clock::now();

//		m->find();
		m->track();
		auto track = std::chrono::high_resolution_clock::now();
//		m->show();
		auto show = std::chrono::high_resolution_clock::now();
		m->send(true);
		auto send = std::chrono::high_resolution_clock::now();
//
//		// миллисекунды оставшиеся до конца фрейма (~30fps)
		int time_left = 1000/30 - 1000*(double)duration_cast<duration<double>>(send - start).count();
		int delay     = time_left/tracker::XTRAPLTN_RATE;
//
//		std::cout << time_left << "\n";
//
		for(int i=0; i<=tracker::XTRAPLTN_RATE-1; i++){
			if(delay<=0) continue;
			int key = cv::waitKey(delay);
			switch (key) {
				case 27: {
					std::cout << "esc key is pressed by user\n";
					exit(0);
				}
			}
			m->send(false);
		}
		auto extr = std::chrono::high_resolution_clock::now();

		printf( "[%#7.4fms] f:%#7.4f t:%#7.4f sh:%#7.4f s:%#7.4f e:%#7.4f <%d>\n",
				(double)1000*duration_cast<duration<double>>(extr - start).count(),
				(double)1000*duration_cast<duration<double>>(frame - start).count(),
				(double)1000*duration_cast<duration<double>>(track - frame).count(),
				(double)1000*duration_cast<duration<double>>(show - track).count(),
				(double)1000*duration_cast<duration<double>>(send - show).count(),
				(double)1000*duration_cast<duration<double>>(extr - send).count(),
				delay);

//		int key = cv::waitKey(1);
//		switch (key) {
//			case 27: {
//				std::cout << "esc key is pressed by user\n";
//				exit(0);
//			}
//		}
	}
}
