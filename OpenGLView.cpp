//OpenGLView.cpp: COpenGLView 클래스의 구현
//
#pragma once
#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "OpenGL.h"
#endif

#include "OpenGLDoc.h"
#include "OpenGLView.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "resource.h"
#define CIRMAX 360.0f
void loadtextureimg();

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COpenGLView

IMPLEMENT_DYNCREATE(COpenGLView, CView)

BEGIN_MESSAGE_MAP(COpenGLView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_LIGHT_DIRECTIONAL, &COpenGLView::OnLightDirectional)
	ON_COMMAND(ID_LIGHT_DIRECTIONAL32774, &COpenGLView::OnLightDirectional32774)
END_MESSAGE_MAP()

// COpenGLView 생성/소멸

COpenGLView::COpenGLView() noexcept
{

	// TODO: 여기에 생성 코드를 추가합니다.

}

COpenGLView::~COpenGLView()
{
}

BOOL COpenGLView::SetDevicePixelFormat(HDC hdc) {
	int pixelformat;


	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_GENERIC_FORMAT |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,
		0,	0,	0,	0,	0,	0,
		8,
		0,
		8,
		0,	0,	0,	0,
		16,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0,
	};

	//choose the best matcing pixel format
	if ((pixelformat = ChoosePixelFormat(hdc, &pfd)) == FALSE) {
		MessageBox(LPCTSTR("ChoosePixelFormet failed"), LPCTSTR("Error"), MB_OK);
		return FALSE;
	}
	//set pixel format to device context
	if (SetPixelFormat(hdc, pixelformat, &pfd) == FALSE) {
		MessageBox(LPCTSTR("SetpixelFormat failed"), LPCTSTR("Error"), MB_OK);
		return FALSE;
	}
	return TRUE;
}
BOOL COpenGLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// COpenGLView 그리기

void COpenGLView::OnDraw(CDC* /*pDC*/)
{
	COpenGLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.

	DrawGLScene();
}


// COpenGLView 인쇄

BOOL COpenGLView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void COpenGLView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void COpenGLView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// COpenGLView 진단

#ifdef _DEBUG
void COpenGLView::AssertValid() const
{
	CView::AssertValid();
}

void COpenGLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COpenGLDoc* COpenGLView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COpenGLDoc)));
	return (COpenGLDoc*)m_pDocument;
}
#endif //_DEBUG


// COpenGLView 메시지 처리기


int COpenGLView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_hDC = GetDC()->m_hDC;

	//set the pixel format
	if (!SetDevicePixelFormat(m_hDC)) {
		return -1;
	}

	//create rendering context and make it current
	m_hglRC = wglCreateContext(m_hDC);
	wglMakeCurrent(m_hDC, m_hglRC);

	//initinalize renderring mode
	InitGL();
	SetTimer(100, 25, NULL);
	return 0;
}


void COpenGLView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	//deselect redering context and delete it
	wglMakeCurrent(m_hDC, NULL);
	wglDeleteContext(m_hglRC);
}

void COpenGLView::OnDraw()
{
	COpenGLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.

	DrawGLScene();
}

void COpenGLView::InitGL(GLvoid) {
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	loadtextureimg();
	speed = 2;
	is_up = is_scaledown = is_scaleup = is_on = is_pause = false;
}

GLuint COpenGLView::GetTexture(LPCWSTR szFilename) {

	loadtexture = auxDIBImageLoad(szFilename);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &texturedplanet);
	glBindTexture(GL_TEXTURE_2D, texturedplanet);	
	glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, loadtexture->sizeX, loadtexture->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, loadtexture->data);

	return (texturedplanet);
}

void COpenGLView::loadtextureimg() {
	glGenTextures(11, Planet);
	
	Planet[0] = GetTexture(_T("./img/2k_sun.bmp"));//태양 texture tchar형 for unicode
	Planet[1] = GetTexture(_T("./img/2k_mercury.bmp"));//수성 texture 
	Planet[2] = GetTexture(_T("./img/2k_venus.bmp"));//금성 texture 
	Planet[3] = GetTexture(_T("./img/2k_earth.bmp"));//화성 texture 
	Planet[4] = GetTexture(_T("./img/2k_moon.bmp"));//달 texture 
	Planet[5] = GetTexture(_T("./img/2k_mars.bmp"));//화성 texture 
	Planet[6] = GetTexture(_T("./img/2k_jupiter.bmp")); //목성 texture 
	Planet[7] = GetTexture(_T("./img/2k_saturn.bmp")); //토성 texture 
	Planet[8] = GetTexture(_T("./img/uranus.bmp")); //천왕성 texture 
	Planet[9] = GetTexture(_T("./img/neptune.bmp")); //해왕성 texture 

	Planet[10] = GetTexture(_T("./img/logo.bmp")); //배경 texture 
}

void COpenGLView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	ReSizeGLScene(cx, cy);
}


void COpenGLView::ReSizeGLScene(GLsizei width, GLsizei height) {
	if (height == 0) height = 1;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(54.0f, (GLfloat)width/(GLfloat)height, 1.0f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void COpenGLView::DrawGLScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	drawbackground();

	if (!is_up) glTranslatef(0.0f, 0.0f,  -400); //위에서 태양계를 바라봄
	else gluLookAt(0, 500, -600 , 0, 0, 0, 0, 1, 0); //0.500.-600에서 원점을 바라봄
	

	if (is_scaleup) glScalef(2.0, 2.0, 2.0); //2배확대
	else if (is_scaledown) glScalef(0.5, 0.5, 0.5); //2배축소
	
	drawplanet(); //행성을 그린다.

	GLfloat light0_ambient[] = { 0.0,0.0,0.0,0.0 }; //태양빛을 받지 않는 뒷면은 어둡게
	GLfloat light0_diffuse[] = { 1.0,1.0,1.0,1.0 }; // 빛을 받는 부분은 밝게
	GLfloat light0_position[] = { 0.0f, 0.0f, 0.0f ,1.0 };//광원의 위치=태양의 위치
	GLfloat light0_specular[] = { 1.0, 1.0,1.0,1.0 };
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	
	SwapBuffers(m_hDC);
}

void COpenGLView::drawbackground(GLvoid) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Planet[10]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-500, -500, -500);
	glTexCoord2f(1.0, 0.0); glVertex3f(500, -500, -500);
	glTexCoord2f(1.0, 1.0); glVertex3f(500, 500, -500);
	glTexCoord2f(0.0, 1.0); glVertex3f(-500, 500, -500);
	glEnd();
}

void COpenGLView::drawplanet(GLvoid) {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

	glPushMatrix();//Sun
	glBindTexture(GL_TEXTURE_2D, Planet[0]);
	glutSolidSphere(75.0f, 40, 40);//태양을 가장 크게 그림
	glPopMatrix();
	//이때 조명을 설정하지 않으면 태양이 어두워짐
	if (is_on) { // light on
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}
	else { //light off
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
	}

	glPushMatrix(); //Mercury		
	glRotatef(revolution[0], 0.0f, 1.0f, 0.0f); //공전
	glTranslatef(92.0f, 0.0f, 0.0f); // 태양에서 92만큼 떨어진곳에 수성을 그림
	glRotatef(rotation[0], 0.0, 0.0, 1.0); //자전
	glBindTexture(GL_TEXTURE_2D, Planet[1]); 
	glutSolidSphere(8.0f, 40, 40); //수성의크기 8
	glPopMatrix();//다시 태양계 좌표

	glPushMatrix();  //Venus
	glRotatef(revolution[1], 0.0f, 1.0f, 0.0f); //공전
	glTranslatef(120.0f, 0.0f, 0.0f); //태양으로부터 120만큼 떨어진 곳에 금성을 그림
	glRotatef(rotation[5], 0.0, 0.0, 1.0); //자전
	glBindTexture(GL_TEXTURE_2D, Planet[2]);
	glutSolidSphere(12.4f, 40, 40);  //금성의크기
	glPopMatrix();

	glPushMatrix(); //Earth
	glRotatef(revolution[2], 0.0f, 1.0f, 0.0f); //지구 공전
	glTranslatef(200.0f, 0.0f, 0.0f); //  50만큼 떨어짐
	glRotatef(rotation[2], 0.0, 1.0, 0.0);
	glBindTexture(GL_TEXTURE_2D, Planet[3]);
	glutSolidSphere(12.0f, 40, 40);

	glPushMatrix();     //달 좌표계
	glRotatef(revolution[3], 0.0f, 1.0f, 0.0f); //공전
	glTranslatef(20.0f, 0.0f, 0.0f); //지구에서 20만큼 떨어진 곳에 달을 그림
	glRotatef(rotation[3], 0.0, 0.0, 1.0); //자전
	glBindTexture(GL_TEXTURE_2D, Planet[4]);
	glutSolidSphere(4.5f, 40, 40); //달 크기 4.5
	glPopMatrix(); //지구 좌표계
	glPopMatrix(); //태양계 좌표계


	glPushMatrix();  //Mars
	glRotatef(revolution[4], 0.0f, 1.0f, 0.0f); //공전
	glTranslatef(250.0f, 0.0f, 0.0f); //태양에서 250만큼 떨어진 곳에 화성그림
	glRotatef(rotation[4], 0.0, 0.0, 1.0); //자전
	glBindTexture(GL_TEXTURE_2D, Planet[5]);
	glutSolidSphere(12.0f, 40, 40);
	glPopMatrix();// 다시 태양계 좌표계로


	glPushMatrix(); //Jupiter
	glRotatef(revolution[5], 0.0f, 1.0f, 0.0f); //공전
	glTranslatef(350.0f, 0.0f, 0.0f); // 태양에서 350 떨어진 곳에 목성 그림
	glRotatef(rotation[5], 0.0, 0.0, 1.0); //자전
	glBindTexture(GL_TEXTURE_2D, Planet[6]);
	glutSolidSphere(37.0f, 40, 40); // 크기 37 태양게에서 가장 큰 행성
	glPopMatrix();//다시 태양계 좌표계


	glPushMatrix(); //Saturn
	glRotatef(revolution[6], 0.0f, 1.0f, 0.0f); //토성 공전
	glTranslatef(450.0f, 0.0f, 0.0f); // 태양으로부터 450 떨어진 곳에 토성을 그림
	gluDisk(gluNewQuadric(), 28.1f, 33.0f, 20, 10); //토성의 반지름보다 조금크게 안쪽, 바깥쪽 반지름을 가진 도넛모양의 토성고리   
	glRotatef(rotation[6], 1.0f, 0.0f, 0.0f);  //자전
	glBindTexture(GL_TEXTURE_2D, Planet[7]);
	glutSolidSphere(23.1f, 40, 40);
	glPopMatrix();//다시 태양계 좌표계

	glPushMatrix();//uranus, 천왕성
	glRotatef(revolution[7], 0.0f, 1.0f, 0.0f);//천왕성 공전
	glTranslatef(630.0f, 0.0f, 0.0f); // 태양으로ㅂ터 630 떨어진 곳에 천왕성그림
	glRotatef(rotation[7], 0.0, 1.0, 0.0); //자전
	glBindTexture(GL_TEXTURE_2D, Planet[8]);//맵핑
	glutSolidSphere(24.0f, 40, 40);	
	glPopMatrix();

	glPushMatrix();//neptune, 해왕성
	glRotatef(revolution[8], 0.0f, 1.0f, 0.0f);//공전
	glTranslatef(700.0f, 0.0f, 0.0f); //가장 먼 곳에 해왕성 그림
	glRotatef(rotation[8], 0.0, 1.0, 0.0); //자전
	glBindTexture(GL_TEXTURE_2D, Planet[9]);
	glutSolidSphere(23.4f, 30, 30);
	glPopMatrix();

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

    glPopMatrix();
}

void COpenGLView::rev_speedchanger(GLfloat revolution[], int rev_speed) {
	for (int i = 0; i < 10; i++) { //공전 속도를 조절한다 rev_speed값이 높아지면 공전속도가 빨라진다.
		revolution[i] += rev_increase[i] * rev_speed;
		if (revolution[i] > CIRMAX) revolution[i] = 0.0f; //초기화
	}
}

void COpenGLView::rot_speedchanger(GLfloat rotation[]) {
	for (int i = 0; i < 10; i++) {// 자전속도를 조절한다. 자전속도는 up, down키에 따라 달라지지 않아 speed가 없다.
		rotation[i] += rot_increase[i];
		if (rotation[i] > CIRMAX) rotation[i] = 0.0f; //초기화
	}
}

void COpenGLView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nChar == VK_UP) speed++; //공전속도 증가
	else if (nChar == VK_DOWN) speed--;
	else if (nChar == VK_RIGHT) is_up = true; //공전속도 감소
	else if (nChar == VK_LEFT) is_up = false; // 위에서 보기
	else if (nChar == VK_SPACE) is_pause =! is_pause; //멈춤

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void COpenGLView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (speed == 0) {
		speed = 1;
		AfxMessageBox(_T("Minimum Speed"));
	}

	else if (speed == 1) {
		if(is_pause)  rev_speedchanger(revolution, 0);
		else {
			rev_speedchanger(revolution, 1);
			rot_speedchanger(rotation);
		}
	}
	else if (speed == 2) {
		if(is_pause) rev_speedchanger(revolution, 0);
		else {
			rev_speedchanger(revolution, 4); //공전 4배
			rot_speedchanger(rotation);
		}
	}

	if (speed == 3) {
		if (is_pause) rev_speedchanger(revolution, 0);
		else {
			rev_speedchanger(revolution, 16); //공전 16배
			rot_speedchanger(rotation);
		}
	}

	if (speed == 4) {
		speed = 3;
		AfxMessageBox(_T("Max Speed"));

	}
	InvalidateRect(NULL, FALSE);
	CView::OnTimer(nIDEvent);
}


BOOL COpenGLView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (zDelta > 0) {//위로 올렸을때
		is_scaleup = true; //확대
		is_scaledown = false;
	}

	else {//아래로 내렸을때
		is_scaleup = false;
		is_scaledown = true; //축소
	}
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void COpenGLView::OnLightDirectional() //light on
{
	is_on = !is_on;
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void COpenGLView::OnLightDirectional32774() //light off
{
	is_on = !is_on;
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}
