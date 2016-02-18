#ifndef ALG_H
#define ALG_H

#include "camera.h"

typedef struct _measure_result{
	float amin;
	float amax;
	float aavg;
	s16 min;
	s16 max;
	s16 avg;

	u16 max_x;
	u16 max_y;

	u16 min_x;
	u16 min_y;
}measure_result;

extern "C"{
	CAMERA_API void MeasurePoint(const measure_point* point, const DataFrame* frame, measure_result* result);
	CAMERA_API void MeasureLine(const measure_line* line, const DataFrame* frame, measure_result* result);
	CAMERA_API void MeasureRectangle(const measure_rectangle* rectangle, const DataFrame* frame, measure_result* result);
	CAMERA_API void MeasureEllipse(const measure_rectangle* ellipse, const DataFrame* frame, measure_result* result);

	CAMERA_API float fixGetVirtualAtmTrans(const FixInfo& fixInfo);
	CAMERA_API void fixTemp(measure_result& mr, float emissivity, const FixInfo& fixInfo, float virtualAtmTrans);
}

#endif