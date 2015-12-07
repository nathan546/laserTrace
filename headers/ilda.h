#define LEshort(a)	((short)(((a) >> 8) & 0xFF)|(((a) << 8) & 0xFF00))	// endian conversion
#define MAX_VECTORS	3000	/* Number of vectors per frame */
#define MAX_FRAMES	1000	/* Number of frames */

typedef struct _vects {
	short f, x, y;
} VECTS;

typedef struct {
	char	sign[7];	// "ILDA", 0, 0, 0
	char	format;		// 0:3D frame, 1:2D frame, 2:Pallete
	char	framename[16];
	short	points;
	short	framenum;
	short	totalframes;
	char	head;
	char	res;
} ILDA;

/* Vector buffers and related variables */
int nPps = 30000, nDelay = 3;
int CurrentFrame, nFrames;
int nVectors, nVectorPrev, nVectorSaved;
int VectorsLst[MAX_FRAMES];
char StrLst[MAX_FRAMES][16];
VECTS FrameLst[MAX_FRAMES][MAX_VECTORS];
VECTS VectLst[MAX_VECTORS], VectLstPrev[MAX_VECTORS];

short startX, startY;
short width, height;
FILE * file3;

void saveILDA(char *fname, int n_from, int n_to);
short compressILDA(short limitIn);
int matrixAt(short x, short y, char * matrix);
void matrixSet(short x, short y, char * matrix, short value);
int mapOutline(char * matrix);
double calculateDistance(short x1, short y1, short x2, short y2);
void traceFromPoint(int y, int x, char * matrix);
void addVector(int f, int x, int y, FILE * file);
