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
	monitor m;

	for (int i = 1; i < argc; ++i) {
		std::string an = argv[i];
		if ((an == "-d") || (an == "--device")) {

			std::string av = argv[++i];
			std::cout << "Open device: " << av << "\n";
			m(std::stoi(av));

		} else if ((an == "-f") || (an == "--file")) {

			std::string av = argv[++i];
			std::cout << "Open file: " << av << "\n";
			m(av);
		}
	}

	if( !m ) m( 0 );

	while( m.players_count() == 0 )
	{
		m.find();
		m.show();
	}
	while(1) {

		m.getFrame();
////		m.find();
		m.track();
		m.show();
//
		m.send();
		int key = 1; //cv::waitKey(10);
		switch (key) {
			case 27: {
				std::cout << "esc key is pressed by user\n";
				exit(0);
			}
		}
	}
}
