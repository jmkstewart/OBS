/********************************************************************************
 Copyright (C) 2012 Hugh Bailey <obs.jim@gmail.com>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
********************************************************************************/


#pragma once


enum DeviceColorType
{
    DeviceOutputType_RGB,
    DeviceOutputType_I420,
    DeviceOutputType_YV12,
};

class DeviceSource : public ImageSource
{
    friend class CapturePin;

    IGraphBuilder           *graph;
    ICaptureGraphBuilder2   *capture;
    IMediaControl           *control;

    IBaseFilter             *deviceFilter;
    CaptureFilter           *captureFilter;

    //---------------------------------

    DeviceColorType colorType;

    bool            bFlipVertical;
    UINT            renderCX, renderCY;

    bool            bReadyToDraw;

    Texture         *texture;
    HANDLE          hSampleMutex;
    XElement        *data;
    bool            bCapturing, bFiltersLoaded;
    IMediaSample    *curSample;
    Shader          *colorConvertShader;

    //---------------------------------

    void PackPlanar(LPBYTE convertBuffer, LPBYTE lpPlanar);

    void FlushSamples()
    {
        OSEnterMutex(hSampleMutex);
        SafeRelease(curSample);
        OSLeaveMutex(hSampleMutex);
    }

    void Receive(IMediaSample *sample);

    bool LoadFilters();
    void UnloadFilters();

    void Start();
    void Stop();

public:
    bool Init(XElement *data);
    ~DeviceSource();

    void UpdateSettings();

    void Preprocess();
    void Render(const Vect2 &pos, const Vect2 &size);

    void BeginScene();
    void EndScene();

    Vect2 GetSize() const {return Vect2(float(renderCX), float(renderCY));}
};
