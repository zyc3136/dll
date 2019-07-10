#pragma once
template <class T> void inline SafeRelease(T** ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}
class CustomTextRenderer : public IDWriteTextRenderer
{
protected:
	ULONG m_cRef;

	ID2D1Factory* m_pD2DFactory;
	ID2D1RenderTarget* m_pRenderTarget;
	ID2D1Brush* m_pTextBodyBrush;
	ID2D1SolidColorBrush* m_pTextOutlineBrush;
	float mStrokeWidth;

public:
	CustomTextRenderer(
		ID2D1Factory* pD2DFactory, ID2D1RenderTarget* pRenderTarget,
		ID2D1Brush* pTextBodyBrush, ID2D1SolidColorBrush* pTextOutlineBrush,
		float strokeWidth = 1.0f) :
		m_pD2DFactory(pD2DFactory),
		m_pRenderTarget(pRenderTarget),
		m_pTextBodyBrush(pTextBodyBrush),
		m_pTextOutlineBrush(pTextOutlineBrush)
	{
		VMProtectBeginMutation("CustomTextRenderer");
		m_pD2DFactory->AddRef();
		m_pRenderTarget->AddRef();
		m_pTextBodyBrush->AddRef();
		m_pTextOutlineBrush->AddRef();
		VMProtectEnd();
	}

	~CustomTextRenderer()
	{
		VMProtectBeginMutation("`CustomTextRenderer");
		SafeRelease(&m_pD2DFactory);
		SafeRelease(&m_pRenderTarget);
		SafeRelease(&m_pTextBodyBrush);
		SafeRelease(&m_pTextOutlineBrush);
		VMProtectEnd();
	}

	STDMETHOD(DrawGlyphRun)(
		void* clientDrawingContext,
		FLOAT                              baselineOriginX,
		FLOAT                              baselineOriginY,
		DWRITE_MEASURING_MODE              measuringMode,
		DWRITE_GLYPH_RUN const* glyphRun,
		DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription,
		IUnknown* clientDrawingEffect
		)
	{
		HRESULT hr = S_OK;

		ID2D1PathGeometry* pPathGeometry = nullptr;
		hr = m_pD2DFactory->CreatePathGeometry(&pPathGeometry);

		ID2D1GeometrySink* pSink = nullptr;
		if (pPathGeometry)
			hr = pPathGeometry->Open(&pSink);

		if (pSink)
			hr = glyphRun->fontFace->GetGlyphRunOutline(
				glyphRun->fontEmSize,
				glyphRun->glyphIndices,
				glyphRun->glyphAdvances,
				glyphRun->glyphOffsets,
				glyphRun->glyphCount,
				glyphRun->isSideways,
				glyphRun->bidiLevel % 2,
				pSink
			);
		if (pSink)
			hr = pSink->Close();

		// Initialize a matrix to translate the origin of the glyph run.
		D2D1::Matrix3x2F const matrix = D2D1::Matrix3x2F(
			1.0f, 0.0f,
			0.0f, 1.0f,
			baselineOriginX, baselineOriginY
		);
		ID2D1TransformedGeometry* pTransformedGeometry = nullptr;
		if (pPathGeometry)
			hr = m_pD2DFactory->CreateTransformedGeometry(pPathGeometry, &matrix, &pTransformedGeometry);

		if (pTransformedGeometry)
			m_pRenderTarget->DrawGeometry(pTransformedGeometry, m_pTextOutlineBrush,1.0);

		if (pTransformedGeometry)
			m_pRenderTarget->FillGeometry(pTransformedGeometry, m_pTextBodyBrush);
		SafeRelease(&pPathGeometry);
		SafeRelease(&pSink);
		SafeRelease(&pTransformedGeometry);
		return hr;
	}

	STDMETHOD(DrawUnderline)(
		void* clientDrawingContext,
		FLOAT                  baselineOriginX,
		FLOAT                  baselineOriginY,
		DWRITE_UNDERLINE const* underline,
		IUnknown* clientDrawingEffect
		)
	{
		D2D1_RECT_F rect = D2D1::RectF(
			0,
			underline->offset,
			underline->width,
			underline->offset + underline->thickness
		);
		ID2D1RectangleGeometry* pRectangleGeometry = NULL;
		HRESULT hr = m_pD2DFactory->CreateRectangleGeometry(
			&rect,
			&pRectangleGeometry
		);
		D2D1::Matrix3x2F const matrix = D2D1::Matrix3x2F(
			1.0f, 0.0f,
			0.0f, 1.0f,
			baselineOriginX, baselineOriginY
		);

		ID2D1TransformedGeometry* pTransformedGeometry = NULL;
		if (SUCCEEDED(hr))
		{
			hr = m_pD2DFactory->CreateTransformedGeometry(
				pRectangleGeometry,
				&matrix,
				&pTransformedGeometry
			);
		}
		m_pRenderTarget->DrawGeometry(
			pTransformedGeometry,
			m_pTextOutlineBrush
		);

		// Fill in the glyph run
		m_pRenderTarget->FillGeometry(
			pTransformedGeometry,
			m_pTextBodyBrush
		);
		SafeRelease(&pRectangleGeometry);
		SafeRelease(&pTransformedGeometry);
		return hr;
	}

	STDMETHOD(DrawStrikethrough)(
		void* clientDrawingContext,
		FLOAT                      baselineOriginX,
		FLOAT                      baselineOriginY,
		DWRITE_STRIKETHROUGH const* strikethrough,
		IUnknown* clientDrawingEffect
		)
	{
			D2D1_RECT_F rect = D2D1::RectF(
				0,
				strikethrough->offset,
				strikethrough->width,
				strikethrough->offset + strikethrough->thickness
			);
			ID2D1RectangleGeometry* pRectangleGeometry = NULL;
			HRESULT hr = m_pD2DFactory->CreateRectangleGeometry(
				&rect,
				&pRectangleGeometry
			);
			D2D1::Matrix3x2F const matrix = D2D1::Matrix3x2F(
				1.0f, 0.0f,
				0.0f, 1.0f,
				baselineOriginX, baselineOriginY
			);

			ID2D1TransformedGeometry* pTransformedGeometry = NULL;
			if (SUCCEEDED(hr))
			{
				hr = m_pD2DFactory->CreateTransformedGeometry(
					pRectangleGeometry,
					&matrix,
					&pTransformedGeometry
				);
			}
			m_pRenderTarget->DrawGeometry(
				pTransformedGeometry,
				m_pTextOutlineBrush
			);

			// Fill in the glyph run
			m_pRenderTarget->FillGeometry(
				pTransformedGeometry,
				m_pTextBodyBrush
			);
			SafeRelease(&pRectangleGeometry);
			SafeRelease(&pTransformedGeometry);
			return hr;
	}

	STDMETHOD(DrawInlineObject)(
		void* clientDrawingContext,
		FLOAT               originX,
		FLOAT               originY,
		IDWriteInlineObject* inlineObject,
		BOOL                isSideways,
		BOOL                isRightToLeft,
		IUnknown* clientDrawingEffect
		)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(IsPixelSnappingDisabled)(
		void* clientDrawingContext,
		BOOL* isDisabled
		)
	{
		*isDisabled = FALSE;
		return S_OK;
	}

	STDMETHOD(GetCurrentTransform)(
		void* clientDrawingContext,
		DWRITE_MATRIX* transform
		)
	{
		m_pRenderTarget->GetTransform(reinterpret_cast<D2D1_MATRIX_3X2_F*>(transform));
		return S_OK;
	}

	STDMETHOD(GetPixelsPerDip)(
		void* clientDrawingContext,
		FLOAT* pixelsPerDip
		)
	{
		float x, yUnused;

		m_pRenderTarget->GetDpi(&x, &yUnused);
		*pixelsPerDip = x / 96;
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void FAR* FAR* ppvObj)
	{
		if (iid == IID_IUnknown /*|| iid == IID_IDWritePixelSnapping || iid == IID_IDWriteTextRenderer*/)
		{
			*ppvObj = this;
			AddRef();
			return NOERROR;
		}
		return E_NOINTERFACE;
	}

	ULONG STDMETHODCALLTYPE AddRef()
	{
		return ++m_cRef;
	}

	ULONG STDMETHODCALLTYPE Release()
	{
		// Decrement the object's internal counter.
		if (0 == --m_cRef)
		{
			delete this;
		}
		return m_cRef;
	}
};
