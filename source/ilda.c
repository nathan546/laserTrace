//Performs compression of frames and vectors
short compressILDA(short limitIn){
    int n, n2, hit;
    int limit = 1;

BEGIN: //Spaghetti code! Could do{ }while it if I really wanted
    hit = 0;
    for (n = 0; n < VectorsLst[0]; n++) {
            if(n+2 < VectorsLst[0] && FrameLst[0][n].f == 1 && FrameLst[0][n+1].f == 1 && FrameLst[0][n+2].f == 1){
                int y = FrameLst[0][n+2].y - FrameLst[0][n].y;
                int x = FrameLst[0][n+2].x - FrameLst[0][n].x;
                double m = (double)y/x;
                double b = FrameLst[0][n].y - m*FrameLst[0][n].x;
                double dist = abs(m*FrameLst[0][n+1].x - FrameLst[0][n+1].y + b) / sqrt(pow(m, 2) + 1);

                if(dist < limit || m == 0){
                    //Linked lists would be quickest
                    VectorsLst[0]--;
                    for(n2 = n+1; n2 < VectorsLst[0]; n2++){
                        FrameLst[0][n2].x = FrameLst[0][n2+1].x;
                        FrameLst[0][n2].y = FrameLst[0][n2+1].y;
                        FrameLst[0][n2].f = FrameLst[0][n2+1].f;
                    }
                    hit = 1;
                }
            }
    }
    if(hit){
        goto BEGIN;
    }if(limit < limitIn){
        limit+=1;
        goto BEGIN;
    }
}

//Returns value at matrix pos
int matrixAt(short x, short y, char * matrix){
    if(y > 0 && x > 0 && y <= height && x <= width){
        return !matrix[(height-y)*(width) + (x-1)];
    }else{
        return 0;
    }
}

//Set value of matrix at pos
void matrixSet(short x, short y, char * matrix, short value){
    if(y > 0 && x > 0 && y <= height && x <= width){
        matrix[(height-y)*width + (x-1)] = !value;
    }
}

//Maps the outline of image into vectors
int mapOutline(char * matrix){
    int i, j;

    for(i = 1; i <= height; i++){
        for(j = 1; j <= width; j++){
            if(matrixAt(j, i, matrix) != 0){
                        startX = j;
                        startY = i;
                        addVector(0, startX, startY, file3);
                        traceFromPoint(i, j, matrix);
            }
        }
    }
}

//Calculates distance between two points
double calculateDistance(short x1, short y1, short x2, short y2){
    return sqrt(pow((y2-y1), 2) + pow((x2-x1), 2));
}

//Start line tracing from point
void traceFromPoint(int y, int x, char * matrix){
    int i, j;
    int y2 = y;
    int x2 = x;
    int furthestLeft = x-1;
    int furthestTop = y-1;
    int found = 0;
    double dist = 0;

    for(i = 0; i < 1*2+1; i++){
        y2 = furthestTop+i;
        for(j = 0; j < 1*2+1; j++){
            x2 = furthestLeft+j;
            if(matrixAt(x2, y2, matrix)){
                y = y2;
                x = x2;
                found = 1;
            }
            matrixSet(x2, y2, matrix, 0);
        }
    }

    if(found){
        addVector(1, x, y, file3);
        traceFromPoint(y, x, matrix);
    }else{

        int radius = 500; //this radius is ridiculous, make parameter
        int newX,newY;
        furthestLeft = x-radius;
        furthestTop = y-radius;
        found = 0;
        dist = 0;

        for(i = 0; i < radius*2+1; i++){
            y2 = furthestTop+i;
            for(j = 0; j < radius*2+1; j++){
                x2 = furthestLeft+j;
                if(matrixAt(x2, y2, matrix)){
                        double temp = calculateDistance(x,y,x2,y2);
                        if((!found || temp <= dist) && temp != 0){
                            newY = y2;
                            newX = x2;
                            dist = temp;
                            found = 1;
                        }
                }
            }
        }

        if(found){
            matrixSet(newX, newY, matrix, 0);
            x = newX;
            y= newY;
            if(dist <= 5){
                addVector(1, x, y, file3);
            }else{
                addVector(0, x, y, file3);
            }
            traceFromPoint(y, x, matrix);
        }
    }

}

//Adds a vector to current frame
void addVector(int f, int x, int y, FILE * file){

    if(width < 1094){
        x += (1094-width)/2;
    }

    if(height < 1094){
        y += (1094-height)/2;
    }

    if(y < 547){
        y = (547-y)*-60;
    }else{
        y = (y-547)*60;
    }

    if(x < 547){
        x = (547-x)*-60;
    }else{
        x = (x-547)*60;
    }

    FrameLst[0][VectorsLst[0]].f = f;
    FrameLst[0][VectorsLst[0]].x = x;
    FrameLst[0][VectorsLst[0]].y = y;
    VectorsLst[0]++;
}

//Outputs frames and vectors to out.ild
void saveILDA(char *fname, int n_from, int n_to)
{
	FILE *fp;
	int n, frm;
	ILDA header;
	short vec[4];

	for (n = n_from; n <= n_to; n++) {
		if (VectorsLst[n] == 0) {
            printf("error1");
			return;
		}
	}

	fp = fopen("out.ild", "wb");
	if (fp == NULL) {
		printf("error2");
		return;
	}

	memset(&header, 0, sizeof(ILDA));
	memcpy(&header.sign[0], "ILDA\0\0\0", 7);
	header.totalframes = LEshort(n_to - n_from + 1);
	vec[2] = 0;

	for (frm = n_from; frm <= n_to; frm++) {
		memcpy(&header.framename[0], &StrLst[frm], 16);
		header.points = LEshort(VectorsLst[frm]);
		fwrite(&header, 1, sizeof(header), fp);
		for (n = 0; n < VectorsLst[frm]; n++) {
			vec[0] = LEshort(FrameLst[frm][n].x);
			vec[1] = LEshort(FrameLst[frm][n].y);
			vec[3] = LEshort((FrameLst[frm][n].f & 1) ? 0x0001 : 0x4000);
			fwrite(vec, 1, sizeof(vec), fp);
		}
		header.framenum = LEshort(LEshort(header.framenum) + 1);
	}
	header.points = 0;
	fwrite(&header, 1, sizeof(header), fp);

	fclose(fp);
	return;
}