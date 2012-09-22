#include <emmintrin.h>
inline void RECTSet(RECT *rc, int left, int top, int right,int bottom ) 
{ 
	_mm_storeu_si128((__m128i*)rc, _mm_set_epi32( bottom, right, top, left ));
}
inline void RECTOffset(RECT *rc, int dx, int dy ) 
{ 
	_mm_storeu_si128((__m128i*)rc, _mm_add_epi32( _mm_set_epi32( dy, dx, dy, dx ),_mm_loadu_si128((__m128i*)rc))); 
}
inline void RECTLefeTopMoveCenter( RECT *rc ) 
{ 
	__m128i r=_mm_loadu_si128((__m128i*)rc);
	_mm_storeu_si128((__m128i*)rc,_mm_add_epi32(r,_mm_srai_epi32(_mm_sub_epi32( _mm_shuffle_epi32( r, _MM_SHUFFLE(3,2,3,2)),_mm_shuffle_epi32( r, _MM_SHUFFLE(1,0,1,0))),1)));
}
inline void RECTLefeTopMoveCenter( RECT *c,RECT *rc  ) 
{ 
	__m128i r=_mm_loadu_si128((__m128i*)rc);
	__m128i s=_mm_loadu_si128((__m128i*)c);
	_mm_storeu_si128((__m128i*)c,_mm_add_epi32(s,_mm_srai_epi32(_mm_sub_epi32( _mm_shuffle_epi32( r, _MM_SHUFFLE(3,2,3,2)),_mm_shuffle_epi32( r, _MM_SHUFFLE(1,0,1,0))),1)));
}
inline void RECTLefeTopMoveRightBottom( RECT *c,RECT *rc  ) 
{ 
	__m128i r=_mm_loadu_si128((__m128i*)rc);
	__m128i s=_mm_loadu_si128((__m128i*)c);
	_mm_storeu_si128((__m128i*)c,_mm_add_epi32(s,_mm_sub_epi32( _mm_shuffle_epi32( r, _MM_SHUFFLE(3,2,3,2)),_mm_shuffle_epi32( r, _MM_SHUFFLE(1,0,1,0)))));
}
inline void RECTCenterMoveLeftTop( RECT *c,RECT *rc  ) 
{ 
	__m128i r=_mm_loadu_si128((__m128i*)rc);
	__m128i s=_mm_loadu_si128((__m128i*)c);
	_mm_storeu_si128((__m128i*)c,_mm_sub_epi32(s,_mm_srai_epi32(_mm_sub_epi32( _mm_shuffle_epi32( r, _MM_SHUFFLE(3,2,3,2)),_mm_shuffle_epi32( r, _MM_SHUFFLE(1,0,1,0))),1)));
}
inline void	RECTReSize ( RECT *rc,int w,int h )
{ 
	_mm_storeu_si128((__m128i*)rc,_mm_add_epi32( _mm_set_epi32( h, w, 0, 0 ),_mm_shuffle_epi32( _mm_loadu_si128((__m128i*)rc),_MM_SHUFFLE(1,0,1,0) ) ));
}
inline void	RECTReSize ( RECT *rc,RECT *d )
{ 
	__m128i r=_mm_loadu_si128((__m128i*)d);
	_mm_storeu_si128((__m128i*)rc,_mm_add_epi32( _mm_sub_epi32( _mm_shuffle_epi32( r, _MM_SHUFFLE(3,2,3,2)),_mm_shuffle_epi32( r, _MM_SHUFFLE(1,0,3,2)) ),_mm_shuffle_epi32( _mm_loadu_si128((__m128i*)rc),_MM_SHUFFLE(1,0,1,0) ) ));
}
inline void	RECTInflate( RECT *rc,int left, int top, int right,int bottom ) 
{ 
	_mm_storeu_si128((__m128i*)rc,_mm_add_epi32( _mm_set_epi32( bottom, right, top, left ),_mm_loadu_si128((__m128i*)rc))); 
}
inline void	RECTIntersect ( RECT *a,const RECT *b,const RECT *c  )
{
	__m128i m0,m1,m2,m3;
	m0=_mm_loadu_si128((__m128i*)b);
	m1=_mm_shuffle_epi32( m0, _MM_SHUFFLE(3,2,3,2));
	m2=_mm_loadu_si128((__m128i*)c);
	m3=_mm_shuffle_epi32( m2, _MM_SHUFFLE(3,2,3,2));
	_mm_storeu_si128((__m128i*)a,
		_mm_unpacklo_epi64 (_mm_max_epi16(_mm_min_epi16(m0, m1), _mm_min_epi16(m2, m3))
			,_mm_min_epi16(_mm_max_epi16(m0, m1), _mm_max_epi16(m2, m3))));
}
inline bool RECTPointIn( const RECT *rc,const POINT &p ) 
{		
	__m128i v=_mm_set_epi32( p.y, p.x, p.y, p.x );
	__m128i r=_mm_loadu_si128((__m128i*)rc);
	return 0 == _mm_movemask_epi8( _mm_unpacklo_epi64(_mm_cmpgt_epi32(r,v),_mm_shuffle_epi32(_mm_cmplt_epi32(r,v),_MM_SHUFFLE(1,0,3,2))) );
}
inline void RECTSize( SIZE *s,const RECT *rc ) 
{		
	__m128i r=_mm_loadu_si128((__m128i*)rc);
	_mm_storel_epi64( (__m128i*)s,_mm_sub_epi32( _mm_shuffle_epi32( r, _MM_SHUFFLE(3,2,3,2)),r ) );
}
inline void RECTDiv( float *f, const RECT *rc1,const RECT *rc2 ) 
{		
	__m128 r=_mm_cvtepi32_ps(_mm_loadu_si128((__m128i*)rc1));
	__m128 c=_mm_cvtepi32_ps(_mm_loadu_si128((__m128i*)rc2));
	_mm_storeu_ps(f,_mm_movehl_ps(_mm_set1_ps(1.0f),_mm_div_ps(_mm_sub_ps(_mm_movehl_ps(r,r),_mm_movelh_ps(r,r)),_mm_sub_ps(_mm_movehl_ps(c,c),_mm_movelh_ps(c,c)))));
}
inline void RECTPointToVector( float *f, const RECT *rc ) 
{	
	_mm_storeu_ps(f,_mm_movelh_ps(_mm_cvtepi32_ps(_mm_loadu_si128((__m128i*)rc)),_mm_set1_ps(0.0f)));
}
inline int RectWidth( RECT &rc ) { return ( rc.right - rc.left ); }
inline int RectHeight( RECT &rc ) { return ( rc.bottom - rc.top ); }