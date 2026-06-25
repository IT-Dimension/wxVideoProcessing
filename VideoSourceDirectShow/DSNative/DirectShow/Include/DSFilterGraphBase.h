
#ifndef __DS_FILTERGRAPH_BASE_H__
#define __DS_FILTERGRAPH_BASE_H__

#include "DSFilter.h"

#define WM_GRAPH_NOTIFY		WM_USER + 0x100
#define WM_SAMPLE_CALLBACK	WM_USER + 0x101
#define WM_VIDEO_SIZE		WM_USER + 0x102
//#define USE_DS_OBJECT

#ifdef _DEBUG
	//#define ROT_DEBUG
#else
	#ifdef ROT_DEBUG
		#undef ROT_DEBUG
	#endif
#endif

#ifdef ROT_DEBUG

class DSYoooROTFilter : public DSFilterBase
{
public:
	DSYoooROTFilter():DSFilterBase(L"@device:sw:{1C41D721-C2B6-46B2-A3E7-2845ED43A8F5}\\{CECB095D-B900-496F-9FDF-9C6EF331468F}")
	{
	}
};


#endif

class DSFilterGraphBase 
#ifdef USE_DS_OBJECT
	: public DSObjectBase
#endif
{
private:
#ifdef ROT_DEBUG
	DSYoooROTFilter * m_pDSYoooROTFilter;
#endif
protected:
	IGraphBuilder   * m_pGraphBuilder;	// graph for rendering
	IMediaControl   * m_pMediaControl;	// main media
	IMediaEventEx	* m_pMediaEventEx;	// to check the stop
	IVideoWindow	* m_pVideoWindow;
	IBasicAudio		* m_pBasicAudio;
	IBasicVideo		* m_pBasicVideo;
	IMediaSeeking	* m_pMediaSeeking;
protected:
	HWND m_hWnd;
	HWND m_hWndNotify;
	bool m_bShouldPreview;
	bool m_bMute;
	int m_iVolume;
protected:
	virtual HRESULT InitInterfaces();
	virtual HRESULT CloseInterfaces();
	virtual void SettingUpVideoWindow();
	virtual HRESULT PreparePlayback();
public:
	virtual HRESULT ProcessGraphMessage();
	virtual HRESULT ProcessStepMessage();
protected:
	HRESULT SetVolume(int nVolume);
	IBaseFilter * GetVideoRenderer();
	IBaseFilter * GetAudioRenderer();
public:
	int get_Volume();
	void set_Volume(int nVolume);
	bool get_Mute();
	void set_Mute(bool bMute);
	bool IsAudioSupported();
	bool IsRunning();
	bool IsPaused();
	HWND get_PreviewWindow();
	void set_PreviewWindow(HWND hWnd);
	HWND get_NotifyWindow();
	void set_NotifyWindow(HWND hWnd);
	bool get_ShouldPreview();
	void set_ShouldPreview(bool bShouldPreview);
	RECT get_VideoSize();
	RECT get_SourceVideoSize();
	void set_VideoSize(RECT _rect);
	REFERENCE_TIME get_Position();
	void set_Position(REFERENCE_TIME _time);
	REFERENCE_TIME get_Duration();
public:
	STDMETHOD(Start)();
	STDMETHOD(Stop)();
	STDMETHOD(Pause)();

	void ResizeVideoWindow();
	void ResizeVideoWindow(RECT rect);
public:
	DSFilterGraphBase();
	virtual ~DSFilterGraphBase();
};

#endif