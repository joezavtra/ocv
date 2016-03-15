/*
 * server.cpp
 *
 *  Created on: 31 янв. 2016 г.
 *      Author: pavelzelenov
 */

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
		m->show();
		auto show = std::chrono::high_resolution_clock::now();
		m->track();
		auto track = std::chrono::high_resolution_clock::now();
		m->send(true);
		auto send = std::chrono::high_resolution_clock::now();

//		for(int i=0; i<=tracker::XTRAPLTN_RATE-1; i++){
//			int key = cv::waitKey(1000/160/tracker::XTRAPLTN_RATE);
//			switch (key) {
//				case 27: {
//					std::cout << "esc key is pressed by user\n";
//					exit(0);
//				}
//			}
//			m->send(false);
//		}
		auto extr = std::chrono::high_resolution_clock::now();

		std::cerr << "f: "   << 1000*duration_cast<duration<double>>(frame - start).count()
				  << "\tsh: " << 1000*duration_cast<duration<double>>(show - frame).count()
				  << "\tt: "  << 1000*duration_cast<duration<double>>(track - show ).count()
				  << "\ts: "  << 1000*duration_cast<duration<double>>(send - track).count()
				  << "\te: "  << 1000*duration_cast<duration<double>>(extr - send).count()
				  << "\n";

		int key = cv::waitKey(100);
		switch (key) {
			case 27: {
				std::cout << "esc key is pressed by user\n";
				exit(0);
			}
		}
	}
}
