#include "pch.h"
#include "MarkDetector.h"

CMarkDetector::CMarkDetector()
{
}

CMarkDetector::~CMarkDetector()
{
	if (m_pBitmapinfo)
	{
		delete[] m_pBitmapinfo;
		m_pBitmapinfo = nullptr;
	}
}

void CMarkDetector::SetPicCtrl(CStatic* pWnd) {
	m_pStatic = pWnd;
}

void CMarkDetector::InitBitmapInfo(int width, int height, int bpp)
{
	if (m_pBitmapinfo)
	{
		delete[] m_pBitmapinfo;
		m_pBitmapinfo = nullptr;
	}

	if (bpp == 8)
		m_pBitmapinfo = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD)];
	else // 24 or 32bit
		m_pBitmapinfo = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO)];

	m_pBitmapinfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_pBitmapinfo->bmiHeader.biPlanes = 1;
	m_pBitmapinfo->bmiHeader.biBitCount = bpp;
	m_pBitmapinfo->bmiHeader.biCompression = BI_RGB;
	m_pBitmapinfo->bmiHeader.biSizeImage = 0;
	m_pBitmapinfo->bmiHeader.biXPelsPerMeter = 0;
	m_pBitmapinfo->bmiHeader.biYPelsPerMeter = 0;
	m_pBitmapinfo->bmiHeader.biClrUsed = 0;
	m_pBitmapinfo->bmiHeader.biClrImportant = 0;

	if (bpp == 8)
	{
		for (int i = 0; i < 256; i++)
		{
			m_pBitmapinfo->bmiColors[i].rgbBlue = (BYTE)i;
			m_pBitmapinfo->bmiColors[i].rgbGreen = (BYTE)i;
			m_pBitmapinfo->bmiColors[i].rgbRed = (BYTE)i;
			m_pBitmapinfo->bmiColors[i].rgbReserved = 0;
		}
	}

	m_pBitmapinfo->bmiHeader.biWidth = width;
	m_pBitmapinfo->bmiHeader.biHeight = -height;
}

// �̹��� ��ó�� 
// mode == 0 �̸� Match Contour(�׷��̽����� + �� + ����ȭ)
// mode == 1 �̸� Edge Simple ���
Mat CMarkDetector::PreprocessImage(const Mat& src)
{
	Mat gray, blur, binary, sobel;
	//�׷��� ������
	if (src.channels() != 1)
		cvtColor(src, gray, COLOR_BGR2GRAY);
	else
		gray = src.clone();

	//����þ� ��
	GaussianBlur(gray, blur, Size(5, 5), 0);

	return blur;
}

Mat CMarkDetector::SobelEdgeBinary(Mat& img)
{
	Mat blur = PreprocessImage(img);

	Mat dx, dy, magnitude_mat, binary;

	// �Һ� ���� ����
	Sobel(blur, dx, CV_32F, 1, 0, 3);
	Sobel(blur, dy, CV_32F, 0, 1, 3);

	//���� ����
	magnitude(dx, dy, magnitude_mat);
	magnitude_mat.convertTo(magnitude_mat, CV_8U);

	//����ȭ
	threshold(magnitude_mat, binary, 50, 255, THRESH_BINARY);
	return binary;
}

//Match_Edge_Simple
Mat CMarkDetector::matchEdgeSimple(Mat& image, Mat& temple)
{
	TickMeter tm;
	tm.start();

	//�̹��� ��ó��
	Mat image_threshold = SobelEdgeBinary(image);
	Mat temple_threshold = SobelEdgeBinary(temple);

	//���ø� ��Ī ����
	Mat result;
	matchTemplate(image_threshold, temple_threshold, result, TM_CCOEFF_NORMED);

	//��Ī ��ġ
	Point matchLoc;
	double maxVal;
	minMaxLoc(result, 0, &maxVal, 0, &matchLoc);

	// ��Ī�� ��ġ ������ 
	Mat matched_image_binary;
	Mat matched_image = image(Rect(matchLoc.x, matchLoc.y, temple.cols, temple.rows));
	Mat matched_image_blur = PreprocessImage(matched_image);
	threshold(matched_image_blur, matched_image_binary, 0, 255, THRESH_BINARY | THRESH_OTSU);

	vector<vector<Point>> matched_contours;
	vector<Vec4i> matched_hierarchy;
	findContours(matched_image_binary, matched_contours, matched_hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	// �߽� ��ǥ ���
	Point2d matchCenter(matchLoc.x + temple.cols / 2.0, matchLoc.y + temple.rows / 2.0);

	tm.stop();
	CString str;
	str.Format(_T("Edge Matching execution time: %.2f ms, \nMatching similarity: %.2f%%"), tm.getTimeMilli(), maxVal * 100.0);
	CLogger::GetInstance().Log(str);

	str.AppendFormat(_T(" (%s)"), getSimilarityLevel(maxVal * 100.0));

	MessageBox(nullptr, str.GetString(), _T("Execution time"), MB_OK);

	//��� �̹��� ���� �� �� ��ȯ
	Mat image_result = image.clone();
	if (image_result.channels() == 1)
		cvtColor(image_result, image_result, COLOR_GRAY2BGR);

	//rectangle(image_result, Rect(matchLoc.x, matchLoc.y, temple.cols, temple.rows), Scalar(0, 0, 255), 2);
	drawContours(image_result, matched_contours, -1, Scalar(255, 255, 0), 2, LINE_8, matched_hierarchy, 0, matchLoc);

	Mat sorted_result = SortMatchedResult(image_result, matchCenter);
	DisplayResultMat(sorted_result);

	return sorted_result;
}

//Match_Contours
Mat CMarkDetector::matchContours(Mat& image, Mat& temple, bool useRotation)
{
	TickMeter tm;
	tm.start();

	// �̹��� ��ó��
	Mat image_binary, image_blur = PreprocessImage(image);
	threshold(image_blur, image_binary, 0, 255, THRESH_BINARY | THRESH_OTSU);

	vector<vector<Point>> image_contours;
	vector<Vec4i> image_hierarchy;
	findContours(image_binary, image_contours, image_hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	double bestScore = DBL_MAX;
	int matchIndex = -1;
	Mat bestRotatedTemple;
	int bestAngle = 0;

	vector<Point> bestTemplateContour;

	//ȸ�� �˻� ����
	if (useRotation) {
		for (int angle = -30; angle <= 30; angle += 5) {
			// ���ø� ȸ��
			Point2f center(temple.cols / 2.0f, temple.rows / 2.0f);
			Mat rotMat = getRotationMatrix2D(center, angle, 1.0);
			Mat rotatedTemple;
			warpAffine(temple, rotatedTemple, rotMat, temple.size(), INTER_LINEAR, BORDER_CONSTANT, Scalar(0));

			Mat temple_blur = PreprocessImage(rotatedTemple);
			Mat temple_binary;
			threshold(temple_blur, temple_binary, 0, 255, THRESH_BINARY | THRESH_OTSU);

			vector<vector<Point>> temple_contours;
			vector<Vec4i> temple_hierarchy;
			findContours(temple_binary, temple_contours, temple_hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);

			if (temple_contours.empty()) continue;

			//���� ū ������
			int maxIdx = 0;
			for (int i = 1; i < temple_contours.size(); ++i)
				if (contourArea(temple_contours[i]) > contourArea(temple_contours[maxIdx]))
					maxIdx = i;

			vector<Point>& templContour = temple_contours[maxIdx];

			for (int i = 0; i < image_contours.size(); ++i) {
				double dist = matchShapes(templContour, image_contours[i], CONTOURS_MATCH_I3, 0);
				if (dist < bestScore) {
					bestScore = dist;
					matchIndex = i;
					bestTemplateContour = templContour;
					bestRotatedTemple = rotatedTemple.clone();
					bestAngle = angle;
				}
			}
		}
	}
	else {
		//���� ���
		Mat temple_blur = PreprocessImage(temple);
		Mat temple_binary;
		threshold(temple_blur, temple_binary, 0, 255, THRESH_BINARY | THRESH_OTSU);

		vector<vector<Point>> temple_contours;
		vector<Vec4i> temple_hierarchy;
		findContours(temple_binary, temple_contours, temple_hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);

		if (!temple_contours.empty()) {
			int maxIdx = 0;
			for (int i = 1; i < temple_contours.size(); ++i)
				if (contourArea(temple_contours[i]) > contourArea(temple_contours[maxIdx]))
					maxIdx = i;

			vector<Point>& templContour = temple_contours[maxIdx];

			for (int i = 0; i < image_contours.size(); ++i) {
				double dist = matchShapes(templContour, image_contours[i], CONTOURS_MATCH_I3, 0);
				if (dist < bestScore) {
					bestScore = dist;
					matchIndex = i;
					bestTemplateContour = templContour;
				}
			}
		}
	}

	// �߽��� ���
	Point2d matchCenter(0, 0);
	if (matchIndex != -1) {
		Moments m = moments(image_contours[matchIndex]);
		if (m.m00 != 0)
			matchCenter = Point2d(m.m10 / m.m00, m.m01 / m.m00);
	}

	tm.stop();

	//���絵 ����
	double similarity = max(0.0, 1.0 - (bestScore / 0.5)) * 100.0;

	CString str;
	str.Format(_T("Contour Matching execution time: %.2f ms, \nRotation Check: %s, \nContour Similarity: %.2f%%"),
		tm.getTimeMilli(),
		useRotation ? _T("ON") : _T("OFF"),
		similarity);
	CLogger::GetInstance().Log(str);
	str.AppendFormat(_T(" (%s)"), getSimilarityLevel(similarity));
	MessageBox(nullptr, str.GetString(), _T("Execution time"), MB_OK);

	// ��� ǥ��
	Mat image_result = image.clone();
	if (image_result.channels() == 1)
		cvtColor(image_result, image_result, COLOR_GRAY2BGR);

	drawContours(image_result, image_contours, matchIndex, Scalar(0, 255, 0), 2, LINE_8, image_hierarchy, 0);
	Mat sorted_result = SortMatchedResult(image_result, matchCenter);
	DisplayResultMat(sorted_result);

	return sorted_result;
}

//matchByYOLO
Mat CMarkDetector::matchByYOLO(Mat& img)
{
	Mat image = img.clone();

	// YOLOv8 ONNX �� ���
	string modelPath = "target_with_others.onnx";

	TickMeter tm;
	tm.start();

	// ��Ʈ��ũ �ε�
	dnn::Net net = dnn::readNetFromONNX(modelPath);

	// �Է� ũ�� (YOLOv8�� ���� 640x640�� ���)
	Size modelShape = Size(640, 640);

	// blob ����
	Mat blob;
	dnn::blobFromImage(image, blob, 1.0 / 255.0, modelShape, Scalar(), true, false);
	net.setInput(blob);

	// �� �߷�
	vector<Mat> outputs;
	net.forward(outputs, net.getUnconnectedOutLayersNames());

	// ��� ó��
	int rows = outputs[0].size[1];
	int dimensions = outputs[0].size[2];
	bool yolov8 = false;
	if (dimensions > rows)
	{
		yolov8 = true;
		rows = outputs[0].size[2];
		dimensions = outputs[0].size[1];
		outputs[0] = outputs[0].reshape(1, dimensions);
		transpose(outputs[0], outputs[0]);
	}

	float* data = (float*)outputs[0].data;

	// ���� ��� ������
	vector<int> class_ids;
	vector<float> confidences;
	vector<Rect> boxes;

	// Ŭ���� �̸�
	vector<string> classes = { "target", "other_left", "other_rigth" };
	float modelScoreThreshold = 0.2f;
	float modelNMSThreshold = 0.45f;

	float x_factor = (float)image.cols / (float)modelShape.width;
	float y_factor = (float)image.rows / (float)modelShape.height;

	for (int i = 0; i < rows; ++i)
	{
		float* row = data + i * dimensions;
		float x = row[0];
		float y = row[1];
		float w = row[2];
		float h = row[3];
		float left = (x - 0.5f * w) * x_factor;
		float top = (y - 0.5f * h) * y_factor;
		float width = w * x_factor;
		float height = h * y_factor;
		float* scores = row + 4;
		Mat scoresMat(1, classes.size(), CV_32FC1, scores);
		Point class_id_point;
		double max_class_score;
		minMaxLoc(scoresMat, 0, &max_class_score, 0, &class_id_point);
		if (max_class_score > modelScoreThreshold)
		{
			class_ids.push_back(class_id_point.x);
			confidences.push_back((float)max_class_score);
			boxes.emplace_back((int)left, (int)top, (int)width, (int)height);
		}
	}

	// NMS (���ִ� ����)
	vector<int> nms_result;
	dnn::NMSBoxes(boxes, confidences, modelScoreThreshold, modelNMSThreshold, nms_result);

	// �ð�ȭ
	const int padding = 10;
	const float label_font_scale = 0.7f;
	const int label_thickness = 0.8;
	const int text_margin = 1;

	Mat image_result = image.clone(); // ���� ����
	Point matchCenter;

	for (int i : nms_result)
	{
		Rect box = boxes[i];
		int class_id = class_ids[i];
		float conf = confidences[i];

		if (class_id == 0) {
			matchCenter = Point(box.x + box.width / 2, box.y + box.height / 2);
		}

		// ROI Ȯ��
		Rect padded_box(box.x - padding, box.y - padding, box.width + 2 * padding, box.height + 2 * padding);
		padded_box &= Rect(0, 0, image.cols, image.rows);
		if (padded_box.width <= 0 || padded_box.height <= 0) continue;

		//ROI �׷��� -> ����þ� �� -> ����ȭ -> ���� ����
		Mat roi = image(padded_box);
		Mat gray, blurred, binary_roi, edges;

		cvtColor(roi, gray, COLOR_BGR2GRAY);
		GaussianBlur(gray, blurred, Size(5, 5), 0);
		threshold(blurred, binary_roi, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
		Canny(binary_roi, edges, 50, 150, 3);

		vector<vector<Point>> contours;
		findContours(edges, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
		drawContours(image_result, contours, -1, Scalar(255, 0, 0), 1, LINE_AA, noArray(), INT_MAX, padded_box.tl());

		// ���̺� �غ�
		string label;
		if (class_id >= 0 && class_id < classes.size()) {
			label = classes[class_id] + ": " + format("%.2f", conf);
		}
		else {
			label = "ID: " + to_string(class_id) + ": " + format("%.2f", conf);
		}

		int baseLine = 0;
		Size label_size = getTextSize(label, FONT_HERSHEY_SIMPLEX, label_font_scale, label_thickness, &baseLine);

		// �� ǥ�� ��ġ ���
		int bg_total_width = label_size.width + 2 * text_margin;
		int bg_total_height = label_size.height + 2 * text_margin;
		int bg_rect_x = box.x + (box.width - bg_total_width) / 2;
		int bg_rect_y = box.y + box.height + text_margin;
		int text_draw_x = bg_rect_x + text_margin;
		int text_draw_y = bg_rect_y + text_margin + (label_size.height - baseLine);

		Rect background_rect(bg_rect_x, bg_rect_y, bg_total_width, bg_total_height);

		// �� ��� �� �ؽ�Ʈ
		if (background_rect.width > 0 && background_rect.height > 0) {
			rectangle(image_result, background_rect, Scalar(255, 255, 255), FILLED);
			if (text_draw_x + label_size.width <= background_rect.x + background_rect.width) {
				putText(image_result, label, Point(text_draw_x, text_draw_y),
					FONT_HERSHEY_SIMPLEX, label_font_scale, Scalar(0, 0, 0), label_thickness, LINE_AA);
			}
		}
	}

	tm.stop();
	CString str;
	str.Format(_T("YOLO Matching execution time: %.2f ms"), tm.getTimeMilli());
	MessageBox(nullptr, str.GetString(), _T("Execution time"), MB_OK);
	CLogger::GetInstance().Log(str);

	DisplayResultMat(image_result);

	return image_result;
}

Mat CMarkDetector::matchByTarget(Mat& img)
{
	Mat image = img.clone();

	// YOLOv8 ONNX �� ���
	string modelPath = "only_target.onnx";

	TickMeter tm;
	tm.start();

	// ��Ʈ��ũ �ε�
	dnn::Net net = dnn::readNetFromONNX(modelPath);

	// �Է� ũ��
	Size modelShape = Size(640, 640);

	// blob ����
	Mat blob;
	dnn::blobFromImage(image, blob, 1.0 / 255.0, modelShape, Scalar(), true, false);
	net.setInput(blob);

	// �� �߷�
	vector<Mat> outputs;
	net.forward(outputs, net.getUnconnectedOutLayersNames());

	// ��� ó��
	int rows = outputs[0].size[1];
	int dimensions = outputs[0].size[2];
	bool yolov8 = false;
	if (dimensions > rows)
	{
		yolov8 = true;
		rows = outputs[0].size[2];
		dimensions = outputs[0].size[1];
		outputs[0] = outputs[0].reshape(1, dimensions);
		transpose(outputs[0], outputs[0]);
	}

	float* data = (float*)outputs[0].data;

	// ���� ��� ������
	vector<int> class_ids;
	vector<float> confidences;
	vector<Rect> boxes;

	// Ŭ���� �̸� (target�� ����)
	vector<string> classes = { "target" };
	float modelScoreThreshold = 0.2f;
	float modelNMSThreshold = 0.45f;

	float x_factor = (float)image.cols / (float)modelShape.width;
	float y_factor = (float)image.rows / (float)modelShape.height;

	for (int i = 0; i < rows; ++i)
	{
		float* row = data + i * dimensions;
		float x = row[0];
		float y = row[1];
		float w = row[2];
		float h = row[3];
		float left = (x - 0.5f * w) * x_factor;
		float top = (y - 0.5f * h) * y_factor;
		float width = w * x_factor;
		float height = h * y_factor;

		float score = row[4];  // ���� Ŭ������ ��� score�� �ٷ� �� ��
		if (score > modelScoreThreshold)
		{
			class_ids.push_back(0); // target Ŭ���� ����
			confidences.push_back(score);
			boxes.emplace_back((int)left, (int)top, (int)width, (int)height);
		}
	}

	// NMS (���ִ� ����)
	vector<int> nms_result;
	dnn::NMSBoxes(boxes, confidences, modelScoreThreshold, modelNMSThreshold, nms_result);

	// �ð�ȭ
	const int padding = 10;
	const float label_font_scale = 0.7f;
	const int label_thickness = 0.8;
	const int text_margin = 1;

	Mat image_result = image.clone();
	Point matchCenter;

	for (int i : nms_result)
	{
		Rect box = boxes[i];
		int class_id = class_ids[i];
		float conf = confidences[i];

		if (class_id == 0) {
			matchCenter = Point(box.x + box.width / 2, box.y + box.height / 2);
		}

		// ROI Ȯ��
		Rect padded_box(box.x - padding, box.y - padding, box.width + 2 * padding, box.height + 2 * padding);
		padded_box &= Rect(0, 0, image.cols, image.rows);
		if (padded_box.width <= 0 || padded_box.height <= 0) continue;

		Mat roi = image(padded_box);
		Mat gray, blurred, binary_roi, edges;

		cvtColor(roi, gray, COLOR_BGR2GRAY);
		GaussianBlur(gray, blurred, Size(5, 5), 0);
		threshold(blurred, binary_roi, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
		Canny(binary_roi, edges, 50, 150, 3);

		vector<vector<Point>> contours;
		findContours(edges, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
		drawContours(image_result, contours, -1, Scalar(255, 0, 0), 1, LINE_AA, noArray(), INT_MAX, padded_box.tl());

		// ���̺� �غ�
		string label = classes[0] + ": " + format("%.2f", conf); // �׻� target

		int baseLine = 0;
		Size label_size = getTextSize(label, FONT_HERSHEY_SIMPLEX, label_font_scale, label_thickness, &baseLine);

		int bg_total_width = label_size.width + 2 * text_margin;
		int bg_total_height = label_size.height + 2 * text_margin;
		int bg_rect_x = box.x + (box.width - bg_total_width) / 2;
		int bg_rect_y = box.y + box.height + text_margin;
		int text_draw_x = bg_rect_x + text_margin;
		int text_draw_y = bg_rect_y + text_margin + (label_size.height - baseLine);

		Rect background_rect(bg_rect_x, bg_rect_y, bg_total_width, bg_total_height);

		if (background_rect.width > 0 && background_rect.height > 0) {
			rectangle(image_result, background_rect, Scalar(255, 255, 255), FILLED);
			if (text_draw_x + label_size.width <= background_rect.x + background_rect.width) {
				putText(image_result, label, Point(text_draw_x, text_draw_y),
					FONT_HERSHEY_SIMPLEX, label_font_scale, Scalar(0, 0, 0), label_thickness, LINE_AA);
			}
		}
	}

	tm.stop();
	CString str;
	str.Format(_T("YOLO Matching execution time: %.2f ms"), tm.getTimeMilli());
	MessageBox(nullptr, str.GetString(), _T("Execution time"), MB_OK);
	CLogger::GetInstance().Log(str);

	Mat sorted_result = SortMatchedResult(image_result, matchCenter);
	DisplayResultMat(sorted_result);
	return sorted_result;
}

Mat CMarkDetector::matchORB(Mat& image, Mat& temple)
{
	TickMeter tm;
	tm.start();

	//�̹��� ��ó��
	Mat image_gray = PreprocessImage(image);
	Mat temple_gray = PreprocessImage(temple);

	//ORB ������ ����
	Ptr<ORB> orb = ORB::create();

	//Ű���ν�Ʈ�� ��ũ���� ����
	vector<KeyPoint> keypoints1, keypoints2;
	Mat descriptors1, descriptors2;
	orb->detectAndCompute(temple_gray, noArray(), keypoints1, descriptors1);
	orb->detectAndCompute(image_gray, noArray(), keypoints2, descriptors2);

	// BFMatcher ���� �� ��Ī
	BFMatcher matcher(NORM_HAMMING);
	vector<DMatch> matches;
	matcher.match(descriptors1, descriptors2, matches);

	// ���� ��Ī�� ���� (�Ÿ��� ª�� ������ ����)
	sort(matches.begin(), matches.end());
	vector<DMatch> good_matches;
	for (int i = 0; i < min(50, (int)matches.size()); i++) {
		good_matches.push_back(matches[i]);
	}

	// ��Ī�� ������ ��ǥ ����
	vector<Point2f> points1, points2;
	for (size_t i = 0; i < good_matches.size(); i++) {
		points1.push_back(keypoints1[good_matches[i].queryIdx].pt);
		points2.push_back(keypoints2[good_matches[i].trainIdx].pt);
	}

	// RANSAC�� ����Ͽ� ȣ��׷��� ���
	Mat mask;
	Mat H = findHomography(points1, points2, RANSAC, 3, mask);

	tm.stop();

	// 8�� �̻��� ���� ��ġ�ϴ� ��츸 ǥ��
	vector<DMatch> inlier_matches;
	vector<Point2f> inlier_points2;
	for (int i = 0; i < mask.rows; i++) {
		if (mask.at<uchar>(i)) {
			inlier_matches.push_back(good_matches[i]);
			inlier_points2.push_back(points2[i]);
		}
	}

	//��Ī ����
	double matchRate = static_cast<double>(inlier_matches.size()) / good_matches.size() * 100.0;

	CString str;
	str.Format(_T("ORB Matching execution time: %.2f ms, \nMatch Similarity: %.2f%%"), tm.getTimeMilli(), matchRate);
	CLogger::GetInstance().Log(str);
	str.AppendFormat(_T(" (%s)"), getSimilarityLevel(matchRate));
	MessageBox(nullptr, str.GetString(), _T("Execution time"), MB_OK);


	// ��Ī�� Ư¡������ ������� �簢�� �׸���
	if (!inlier_points2.empty()) {
		// ��Ī�� Ư¡������ �ּ�/�ִ� ��ǥ ���
		float min_x = FLT_MAX, min_y = FLT_MAX;
		float max_x = -FLT_MAX, max_y = -FLT_MAX;

		for (const auto& pt : inlier_points2) {
			min_x = min(min_x, pt.x);
			min_y = min(min_y, pt.y);
			max_x = max(max_x, pt.x);
			max_y = max(max_y, pt.y);
		}

		// �簢���� �ʺ�� ���� ���
		float width = max_x - min_x;
		float height = max_y - min_y;

		// ���� ������ ���� ���� ���� (20% Ȯ��)
		float padding_ratio = 0.2f;
		float padding_x = width * padding_ratio;
		float padding_y = height * padding_ratio;

		// ��ǥ�� ������ ��ȯ�ϰ� ���� ���� �߰�
		int x1 = static_cast<int>(min_x - padding_x);
		int y1 = static_cast<int>(min_y - padding_y);
		int x2 = static_cast<int>(max_x + padding_x);
		int y2 = static_cast<int>(max_y + padding_y);

		// ������ �̹��� ������ ����� �ʵ��� ����
		x1 = max(0, x1);
		y1 = max(0, y1);
		x2 = min(image_gray.cols - 1, x2);
		y2 = min(image_gray.rows - 1, y2);

		// �簢�� ���� �߶󳻱�
		Mat roi = image_gray(Rect(x1, y1, x2 - x1, y2 - y1));

		// ��Ī�� ������ �߽��� ���
		// �簢���� �߽������� ���
		Point2d matchCenter(0, 0);
		matchCenter = Point2d((x1 + x2) / 2.0, (y1 + y2) / 2.0);

		// ���� ����
		Mat edges;
		Canny(roi, edges, 50, 150);

		// ������ ����
		vector<vector<Point>> contours;
		findContours(edges, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

		// �������� ���� �̹��� ��ǥ�� ��ȯ
		for (auto& contour : contours) {
			for (auto& point : contour) {
				point.x += x1;
				point.y += y1;
			}
		}

		// ���� �̹����� �������� �߽��� �׸���
		Mat image_result = image.clone();
		drawContours(image_result, contours, -1, Scalar(0, 255, 255), 2);

		Mat sorted_result = SortMatchedResult(image_result, matchCenter);
		DisplayResultMat(sorted_result);
		return sorted_result;
	}

	// ��Ī�� ���� ��� ���� �̹��� ��ȯ
	return image.clone();
}


// ��� �̹��� ����
Mat CMarkDetector::SortMatchedResult(const Mat& image_result, Point2d matchCenter)
{
	CClientDC dc(m_pStatic);
	CRect rect;
	m_pStatic->GetClientRect(&rect);

	// �߽��� ǥ�� (������ ������)
	circle(image_result, matchCenter, 5, Scalar(0, 0, 255), -1);

	// �߽� ���
	Point2d imageCenter(image_result.cols / 2.0, image_result.rows / 2.0);
	double offsetX = imageCenter.x - matchCenter.x;
	double offsetY = imageCenter.y - matchCenter.y;

	// ��ȯ ���
	Mat translationMat = (Mat_<double>(2, 3) << 1, 0, offsetX, 0, 1, offsetY);
	Mat image_result_sort;
	warpAffine(image_result, image_result_sort, translationMat, image_result.size(), INTER_LINEAR, BORDER_CONSTANT, Scalar(0, 0, 0));

	return image_result_sort;
}

void CMarkDetector::DisplayResultMat(const Mat& img)
{
	Mat image = img.clone();

	CRect rect;
	m_pStatic->GetClientRect(&rect);

	CImage cimg;
	cimg.Create(image.cols, image.rows, 24);

	uchar* pDst = (uchar*)cimg.GetBits();
	int step = cimg.GetPitch();
	for (int y = 0; y < image.rows; y++)
	{
		memcpy(pDst + y * step, image.ptr(y), image.cols * 3);
	}

	CClientDC dc(m_pStatic);
	m_pStatic->Invalidate();
	cimg.StretchBlt(dc.m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);
}

CString CMarkDetector::getSimilarityLevel(double similarity)
{
	if (similarity > 80.0)
		return _T("��");
	else if (similarity > 50.0)
		return _T("��");
	else
		return _T("��");
}