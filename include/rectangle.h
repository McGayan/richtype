class CRectangle
{
private:
	double pointData[8];

public:
	CRectangle();


	double GetMaxX() {
		double maxX = pointData[0];
		for(int i=2; i<7; i+=2) {
			if(maxX < pointData[i])
				maxX = pointData[i];
		}
		return maxX;
	}
	
	double GetMinX() {
		double minX = pointData[0];
		for(int i=2; i<7; i+=2) {
			if(minX > pointData[i])
				minX = pointData[i];
		}
		return minX;
	}
	
	double GetMinY() {
		double minY = pointData[1];
		for(int i=3; i<7; i+=2) {
			if(minY > pointData[i])
				minY = pointData[i];
		}
		return minY;
	}
	
	double GetMaxY() {
		double maxY = pointData[1];
		for(int i=3; i<7; i+=2) {
			if(maxY < pointData[i])
				maxY = pointData[i];
		}
		return maxY;
	}

	double GetX0() {return pointData[0]; }
	double GetX1() {return pointData[2]; }
	double GetX2() {return pointData[4]; }
	double GetX3() {return pointData[6]; }
	double GetY0() {return pointData[1]; }
	double GetY1() {return pointData[3]; }
	double GetY2() {return pointData[5]; }
	double GetY3() {return pointData[7]; }
	double* GetPoints() {return pointData; }
	
	void SetX0(double val) {pointData[0] = val;}
	void SetX1(double val) {pointData[2] = val;}
	void SetX2(double val) {pointData[4] = val;}
	void SetX3(double val) {pointData[6] = val;}
	void SetY0(double val) {pointData[1] = val;}
	void SetY1(double val) {pointData[3] = val;}
	void SetY2(double val) {pointData[5] = val;}
	void SetY3(double val) {pointData[7] = val;}
	
	void SetMinMax(double minX, double minY, double maxX, double maxY) {
		pointData[0] = minX;
		pointData[1] = minY;
		
		pointData[2] = maxX;
		pointData[3] = minY;
		
		pointData[4] = maxX;
		pointData[5] = maxY;
		
		pointData[6] = minX;
		pointData[7] = maxY;
	}
	
};
