#pragma once

// Importation des fichier d'opencv
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>
#include <chrono> 

using namespace std;
using namespace cv;
using namespace chrono; 


class Video_Operation
{
public:


	// Default constructor.
	Video_Operation();

	// Main function.
	void Video_Process(Video_Operation Obj);

private:

};

// Constructeur par default
Video_Operation::Video_Operation()
{

}

// Methode pour traité plusieur image d'un coup (flux).
void Video_Operation::Video_Process(Video_Operation Obj) {
	
	int threshold;
	string filename, filename_out;

	cout << "\n Please give a detection threshold (Commonly used : 20-60) : ";
	cin >> threshold;




	// Imortation of the video by the user.
	cout << "\n Please spercify the full path to the video (Ex : C:/Users/Desktop/filename.mp4. or you can Drag and Drop) : \n";
	cin >> filename;
	VideoCapture video_train(filename);

	cout << "\n Please spercify the name you want for your output video (Ex : Super_Video) : ";
	cin >> filename_out;

	Mat Res;
	video_train >> Res;
	int width = (int)(Res.cols);
	int heigth = (int)(Res.rows);

	VideoWriter video("Output_dir/Out_" + filename_out + ".mp4", VideoWriter::fourcc('m', 'p', '4', 'v'), 30, Size(width, heigth));

	
	
	// Check if file opened successfully
	if (!video_train.isOpened()) {
		cout << "\n Error no video file found\n\n";
		waitKey(5000);
		exit(999);
	}

	// Used to test the performance.
	auto start = high_resolution_clock::now();

	vector<KeyPoint>	keypoint_ch_s_query, keypoint_ch_v_query,
		keypoint_ch_s_train, keypoint_ch_v_train, keypoints;

	Mat	color_descriptor_all_ch,
		color_descriptor_ch_s_query, color_descriptor_ch_v_query,
		color_descriptor_ch_s_train, color_descriptor_ch_v_train;

	Mat img_matches;
	Mat frame_train, frame_train_sv, frame_query, frame_query_sv, frame_query_cntr;
	int frm_query = -1;
	int frm_train = 0;

	Ptr<ORB> ORB_Detector = ORB::create(24, 1.1f, 3, 64, 0, 2, ORB::FAST_SCORE, 64, threshold);


	FlannBasedMatcher FLANN_Matcher = cv::FlannBasedMatcher(makePtr<flann::LshIndexParams>(12, 20, 2));
	vector<vector<DMatch>> keypoint_matches_FLANN_s;
	vector<vector<DMatch>> keypoint_matches_FLANN_v;
	vector<DMatch> empty, good_keypoint_matches;
	
	while (1) {

		frame_query_cntr.copyTo(frame_query);
		frm_query++;
		// Capture frame-by-frame
		video_train >> frame_train;
		frm_train++;
		frame_train.copyTo(frame_query_cntr);

		// If the frame is empty, break immediately
		if (frame_query.empty() == false) {
			if (frame_train.empty() == false) {

				vector<Mat> SV_channels_query, SV_channels_train;
				Mat  img_matches_s, img_matches_v;
				Mat img_matches_FLANN;


				cvtColor(frame_query, frame_query_sv, COLOR_BGR2HSV);
				cvtColor(frame_train, frame_train_sv, COLOR_BGR2HSV);


				split(frame_query_sv, SV_channels_query);
				split(frame_train_sv, SV_channels_train);


				ORB_Detector->detectAndCompute(SV_channels_query[1], noArray(), keypoint_ch_s_query, color_descriptor_ch_s_query);
				ORB_Detector->detectAndCompute(SV_channels_query[2], noArray(), keypoint_ch_v_query, color_descriptor_ch_v_query);

				ORB_Detector->detectAndCompute(SV_channels_query[1], noArray(), keypoint_ch_s_train, color_descriptor_ch_s_train);
				ORB_Detector->detectAndCompute(SV_channels_query[2], noArray(), keypoint_ch_v_train, color_descriptor_ch_v_train);
				

				vector<DMatch> keypoint_matches_FLANN_ch_s;

				if (color_descriptor_ch_s_query.empty() == true || color_descriptor_ch_s_train.empty() == true) {

				}
				else {
					try {
						FLANN_Matcher.knnMatch(color_descriptor_ch_s_query, color_descriptor_ch_s_train, keypoint_matches_FLANN_s, 2);
						
						const float ratio_thresh = 0.7f;


						for (size_t i = 0; i < keypoint_matches_FLANN_s.size(); i++)
						{
							if (keypoint_matches_FLANN_s[i][0].distance < ratio_thresh * keypoint_matches_FLANN_s[i][1].distance)
							{
								keypoint_matches_FLANN_ch_s.push_back(keypoint_matches_FLANN_s[i][0]);
							}
						}
					}
					catch (const std::exception&) {

					}
				}

				vector<DMatch> keypoint_matches_FLANN_ch_v;

				if (color_descriptor_ch_v_query.empty() == true || color_descriptor_ch_v_train.empty() == true) {
				}
				else {
					try {
						FLANN_Matcher.knnMatch(color_descriptor_ch_v_query, color_descriptor_ch_v_train, keypoint_matches_FLANN_v, 2);

						const float ratio_thresh = 0.7f;


						for (size_t i = 0; i < keypoint_matches_FLANN_v.size(); i++)
						{
							if (keypoint_matches_FLANN_v[i][0].distance < ratio_thresh * keypoint_matches_FLANN_v[i][1].distance)
							{
								keypoint_matches_FLANN_ch_v.push_back(keypoint_matches_FLANN_v[i][0]);
							}
						}
					}
					catch (const std::exception&) {

					}
				}

				if (keypoint_matches_FLANN_ch_s.size() >= 4) {
					drawMatches(frame_query, keypoint_ch_s_query, frame_train, keypoint_ch_s_train, keypoint_matches_FLANN_ch_s, img_matches_s, Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
				}
				if (keypoint_matches_FLANN_ch_v.size() >= 4) {
					drawMatches(frame_query, keypoint_ch_v_query, frame_train, keypoint_ch_v_train, keypoint_matches_FLANN_ch_v, img_matches_v, Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
				}


				if (img_matches_s.empty() == true || img_matches_v.empty() == true) {
				}
				else if (img_matches_s.empty() == true || img_matches_v.empty() == false) {
					img_matches_v.copyTo(img_matches);
					resize(img_matches, img_matches, Size(width, heigth));
					video.write(img_matches);
				}
				else if (img_matches_s.empty() == false || img_matches_v.empty() == true) {
					img_matches_s.copyTo(img_matches);
					resize(img_matches, img_matches, Size(width, heigth));
					video.write(img_matches);
				}
				else {
					add(img_matches_s / 2, img_matches_v / 2, img_matches);
					resize(img_matches, img_matches, Size(width, heigth));
					video.write(img_matches);
				}
					
				cout << "|";
	


				/* For live visioning.
				namedWindow("Live", WINDOW_FREERATIO);
				try
				{

					imshow("Live", img_matches);
				}
				catch (const std::exception&)
				{

				}
				*/
				waitKey(1);
			}

			else {
				break;
			}
		}
	}
	// When everything done, release the video capture object
	video_train.release();

	// Closes all the frames
	destroyAllWindows();
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<seconds>(stop - start);
	double fps = ((double)frm_train) / (duration.count());
	cout << "\n\n Duree d'execution du programme : " << duration.count() << "s\n\n Soit : " << (int)fps << " FPS.\n";
}

