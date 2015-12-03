////////////////////////////////////////////////////////////////////////////
// modified version of the maya_fur that ships with 3Delight
///////////////////////////////////////////////////////////////////////////
color dz_curve_hair (
		  vector V;
		  float Ks;
		  float roughness;
		  float Kd;
	      color basecolor;
		  color tipcolor;
		  color specularcolor;
	     )
{
	float sharpness = 1/roughness;

	vector T = normalize (dPdv);
    color Cspec=0, Cdiff=0;
    float cosang;
	normal Nn = normalize(N);
	vector Ln, b;
	
	illuminance( P ) {
		Ln = normalize( L );
		b = normalize(normalize(Ln^T)^T);
		Cdiff += -Ln.b * Cl;
		cosang = cos (abs (acos (T.Ln) - acos (-T.V)));
		Cspec += pow(cosang, sharpness) * Cl;
	}
	
	Cdiff *= mix(basecolor, tipcolor, v) * Kd;
	Cspec *= specularcolor * Ks;
	return Cdiff + Cspec;
}