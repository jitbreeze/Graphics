
// OpenGLView.h: COpenGLView 클래스의 인터페이스
//
#pragma once
#include <gl/GL.h>
#include <gl/glut.h>
#include "resource.h"

class COpenGLView : public CView
{
protected: // serialization에서만 만들어집니다.
	COpenGLView() noexcept;
	DECLARE_DYNCREATE(COpenGLView)

// 특성입니다.
public:
	COpenGLDoc* GetDocument() const;

// 작업입니다.
public:
	HDC		m_hDC;
	HGLRC   m_hglRC;
	AUX_RGBImageRec*	loadtexture;
	GLuint				texturedplanet;

	// revolution (공전)
	GLfloat revolution[10] = {0,0,0,0,0,0,0,0,0};
	GLfloat rev_increase[10] = {1.1714, 0.5722, 0.3244, 2.5, 0.1277, 0.05, 0.025, 0.01, 0.005 };

	// rotation(자전)
	GLfloat rotation[9] = { 0,0,0,0,0,0,0,0,0 };
	GLfloat rot_increase[9] = { 119.9234, 30, 0.5434, 1.2340,  45, 0.1678, 0.0834, 0.0324, 0.002 };
	GLuint scale=1;
	GLuint speed;
	GLuint GetTexture(LPCWSTR szFilename);
	GLuint Planet[11];
	bool is_up, is_scaledown , is_scaleup, is_on, is_pause;

public:
	BOOL SetDevicePixelFormat(HDC hdc);
	void InitGL(void);
	void ReSizeGLScene(GLsizei width, GLsizei height);
	void DrawGLScene(void);
	void drawplanet(void);
	void drawbackground(void);
	void setzero(float angle);
	void loadtextureimg(void);
	void rev_speedchanger(GLfloat revolution[], int rev_speed);
	void rot_speedchanger(GLfloat revolution[]);

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~COpenGLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnDraw();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLightDirectional();
	afx_msg void OnLightDirectional32774();
};

#ifndef _DEBUG  // OpenGLView.cpp의 디버그 버전
inline COpenGLDoc* COpenGLView::GetDocument() const
   { return reinterpret_cast<COpenGLDoc*>(m_pDocument); }
#endif

