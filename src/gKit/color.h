
#ifndef _COLOR_H
#define _COLOR_H

struct Color
{
    float r, g, b, a;
};

Color make_color( const float r, const float g, const float b );
Color make_alpha_color( const float r, const float g, const float b, const float a );
Color make_opaque_color( const Color& color );

Color operator+ ( const Color& a, const Color& b );
Color operator- ( const Color& a, const Color& b );
Color operator- ( const Color& c );
Color operator* ( const Color& a, const Color& b );
Color operator* ( const Color& c, const float k );
Color operator* ( const float k, const Color& c );
Color operator/ ( const Color& a, const Color& b );
Color operator/ ( const float k, const Color& c );
Color operator/ ( const Color& c, const float k );

#endif