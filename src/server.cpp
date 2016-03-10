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

		m->getFrame();
//		m->find();
		m->show();
		m->track();
//		m->send(true);
		for(int i=0; i<=tracker::XTRAPLTN_RATE-1; i++){
//			std::cerr << "";
			m->send(false);
			int key = cv::waitKey(1000/60/tracker::XTRAPLTN_RATE);
			switch (key) {
				case 27: {
					std::cout << "esc key is pressed by user\n";
					exit(0);
				}
			}
		}
	}
}
