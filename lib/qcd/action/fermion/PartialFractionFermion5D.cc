#include <Grid.h>
namespace Grid {
  namespace QCD {

    void  PartialFractionFermion5D::Mdir (const LatticeFermion &psi, LatticeFermion &chi,int dir,int disp){
      // this does both dag and undag but is trivial; make a common helper routing

      int sign = 1;

      DhopDir(psi,chi,dir,disp);

      int nblock=(Ls-1)/2;
      for(int b=0;b<nblock;b++){
	int s = 2*b;
	ag5xpby_ssp(chi,-scale,chi,0.0,chi,s,s); 
	ag5xpby_ssp(chi, scale,chi,0.0,chi,s+1,s+1); 
      }
      ag5xpby_ssp(chi,p[nblock]*scale/amax,chi,0.0,chi,Ls-1,Ls-1);

    }
    void   PartialFractionFermion5D::Meooe_internal(const LatticeFermion &psi, LatticeFermion &chi,int dag)
    {
      // this does both dag and undag but is trivial; make a common helper routing
      int sign = dag ? (-1) : 1;

      if ( psi.checkerboard == Odd ) {
	DhopEO(psi,chi,DaggerNo);
      } else {
	DhopOE(psi,chi,DaggerNo);
      }

      int nblock=(Ls-1)/2;
      for(int b=0;b<nblock;b++){
	int s = 2*b;
	ag5xpby_ssp(chi,-scale,chi,0.0,chi,s,s); 
	ag5xpby_ssp(chi, scale,chi,0.0,chi,s+1,s+1); 
      }
      ag5xpby_ssp(chi,p[nblock]*scale/amax,chi,0.0,chi,Ls-1,Ls-1);
    }

    void   PartialFractionFermion5D::Mooee_internal(const LatticeFermion &psi, LatticeFermion &chi,int dag)
    {
      // again dag and undag are trivially related
      int sign = dag ? (-1) : 1;
      
      int nblock=(Ls-1)/2;
      for(int b=0;b<nblock;b++){
	
	int s = 2*b;
	RealD pp = p[nblock-1-b];
	RealD qq = q[nblock-1-b];
	
	// Do each 2x2 block aligned at s and multiplies Dw site diagonal by G5 so Hw
	ag5xpby_ssp(chi,-dw_diag*scale,psi,amax*sqrt(qq)*scale,psi, s  ,s+1); 
	ag5xpby_ssp(chi, dw_diag*scale,psi,amax*sqrt(qq)*scale,psi, s+1,s);
	axpby_ssp  (chi, 1.0, chi,sqrt(amax*pp)*scale*sign,psi,s+1,Ls-1);
      }
      
      {
	RealD R=(1+mass)/(1-mass);
	//R g5 psi[Ls-1] + p[0] H
	ag5xpbg5y_ssp(chi,R*scale,psi,p[nblock]*scale*dw_diag/amax,psi,Ls-1,Ls-1);
	
	for(int b=0;b<nblock;b++){
	  int s = 2*b+1;
	  RealD pp = p[nblock-1-b];
	  axpby_ssp(chi,1.0,chi,-sqrt(amax*pp)*scale*sign,psi,Ls-1,s);
	}
      }
    }

    void   PartialFractionFermion5D::MooeeInv_internal(const LatticeFermion &psi, LatticeFermion &chi,int dag)
    {
      int sign = dag ? (-1) : 1;

      LatticeFermion tmp(psi._grid);
      
      ///////////////////////////////////////////////////////////////////////////////////////
      //Linv
      ///////////////////////////////////////////////////////////////////////////////////////
      int nblock=(Ls-1)/2;

      axpy(chi,0.0,psi,psi); // Identity piece
      
      for(int b=0;b<nblock;b++){
	int s = 2*b;
	RealD pp = p[nblock-1-b];
	RealD qq = q[nblock-1-b];
	RealD coeff1=sign*sqrt(amax*amax*amax*pp*qq) / ( dw_diag*dw_diag + amax*amax* qq);
	RealD coeff2=sign*sqrt(amax*pp)*dw_diag / ( dw_diag*dw_diag + amax*amax* qq); // Implicit g5 here
	axpby_ssp  (chi,1.0,chi,coeff1,psi,Ls-1,s);
	axpbg5y_ssp(chi,1.0,chi,coeff2,psi,Ls-1,s+1);
      }
      
      ///////////////////////////////////////////////////////////////////////////////////////
      //Dinv (note D isn't really diagonal -- just diagonal enough that we can still invert)
      // Compute Seeinv (coeff of gamma5)
      ///////////////////////////////////////////////////////////////////////////////////////
      RealD R=(1+mass)/(1-mass);
      RealD Seeinv = R + p[nblock]*dw_diag/amax;
      for(int b=0;b<nblock;b++){
	Seeinv += p[nblock-1-b]*dw_diag/amax / ( dw_diag*dw_diag/amax/amax + q[nblock-1-b]);
      }    
      Seeinv = 1.0/Seeinv;
      
      for(int b=0;b<nblock;b++){
	int s = 2*b;
	RealD pp = p[nblock-1-b];
	RealD qq = q[nblock-1-b];
	RealD coeff1=dw_diag / ( dw_diag*dw_diag + amax*amax* qq); // Implicit g5 here
	RealD coeff2=amax*sqrt(qq) / ( dw_diag*dw_diag + amax*amax* qq);
	ag5xpby_ssp  (tmp,-coeff1,chi,coeff2,chi,s,s+1);
	ag5xpby_ssp  (tmp, coeff1,chi,coeff2,chi,s+1,s);
      }
      ag5xpby_ssp  (tmp, Seeinv,chi,0.0,chi,Ls-1,Ls-1);
      
      ///////////////////////////////////////////////////////////////////////////////////////
      // Uinv
      ///////////////////////////////////////////////////////////////////////////////////////
      for(int b=0;b<nblock;b++){
	int s = 2*b;
	RealD pp = p[nblock-1-b];
	RealD qq = q[nblock-1-b];
	RealD coeff1=-sign*sqrt(amax*amax*amax*pp*qq) / ( dw_diag*dw_diag + amax*amax* qq);
	RealD coeff2=-sign*sqrt(amax*pp)*dw_diag / ( dw_diag*dw_diag + amax*amax* qq); // Implicit g5 here
	axpby_ssp  (chi,1.0/scale,tmp,coeff1/scale,tmp,s,Ls-1);
	axpbg5y_ssp(chi,1.0/scale,tmp,coeff2/scale,tmp,s+1,Ls-1);
      }
      axpby_ssp  (chi, 1.0/scale,tmp,0.0,tmp,Ls-1,Ls-1);
    }

    void   PartialFractionFermion5D::M_internal(const LatticeFermion &psi, LatticeFermion &chi,int dag)
    {
      LatticeFermion D(psi._grid);
  
      int sign = dag ? (-1) : 1;

      // For partial frac Hw case (b5=c5=1) chroma quirkily computes
      //
      // Conventions for partfrac appear to be a mess.
      // Tony's Nara lectures have
      //
      // BlockDiag(  H/p_i  1             | 1       )    
      //          (  1      p_i H / q_i^2 | 0       )  
      //           ---------------------------------
      //           ( -1      0                | R  +p0 H  )
      //
      //Chroma     ( -2H    2sqrt(q_i)    |   0         )
      //           (2 sqrt(q_i)   2H      |  2 sqrt(p_i) )
      //           ---------------------------------
      //           ( 0     -2 sqrt(p_i)   |  2 R gamma_5 + p0 2H
      //
      // Edwards/Joo/Kennedy/Wenger
      //
      // Here, the "beta's" selected by chroma to scale the unphysical bulk constraint fields
      // incorporate the approx scale factor. This is obtained by propagating the
      // scale on "H" out to the off diagonal elements as follows:
      //
      // BlockDiag(  H/p_i  1             | 1       ) 
      //          (  1      p_i H / q_i^2 | 0       )  
      //           ---------------------------------
      //          ( -1      0                | R  + p_0 H  )
      //
      // becomes:
      // BlockDiag(  H/ sp_i  1               | 1             ) 
      //          (  1      sp_i H / s^2q_i^2 | 0             )  
      //           ---------------------------------
      //           ( -1      0                | R + p_0/s H   )
      //
      //
      // This is implemented in Chroma by
      //           p0' = p0/approxMax
      //           p_i' = p_i*approxMax
      //           q_i' = q_i*approxMax*approxMax
      //
      // After the equivalence transform is applied the matrix becomes
      // 
      //Chroma     ( -2H    sqrt(q'_i)    |   0         )
      //           (sqrt(q'_i)   2H       |   sqrt(p'_i) )
      //           ---------------------------------
      //           ( 0     -sqrt(p'_i)    |  2 R gamma_5 + p'0 2H
      //
      //     =     ( -2H    sqrt(q_i)amax    |   0              )
      //           (sqrt(q_i)amax   2H       |   sqrt(p_i*amax) )
      //           ---------------------------------
      //           ( 0     -sqrt(p_i)*amax   |  2 R gamma_5 + p0/amax 2H
      //

      DW(psi,D,DaggerNo); 

      int nblock=(Ls-1)/2;
      for(int b=0;b<nblock;b++){
	
	int s = 2*b;
	double pp = p[nblock-1-b];
	double qq = q[nblock-1-b];
	
	// Do each 2x2 block aligned at s and
	ag5xpby_ssp(chi,-1.0*scale,D,amax*sqrt(qq)*scale,psi, s  ,s+1); // Multiplies Dw by G5 so Hw
	ag5xpby_ssp(chi, 1.0*scale,D,amax*sqrt(qq)*scale,psi, s+1,s);
	
	// Pick up last column
	axpby_ssp  (chi, 1.0, chi,sqrt(amax*pp)*scale*sign,psi,s+1,Ls-1);
      }
	
      {
	double R=(1+this->mass)/(1-this->mass);
	//R g5 psi[Ls] + p[0] H
	ag5xpbg5y_ssp(chi,R*scale,psi,p[nblock]*scale/amax,D,Ls-1,Ls-1);
	
	for(int b=0;b<nblock;b++){
	  int s = 2*b+1;
	  double pp = p[nblock-1-b];
	  axpby_ssp(chi,1.0,chi,-sqrt(amax*pp)*scale*sign,psi,Ls-1,s);
	}
      }

    }

    RealD  PartialFractionFermion5D::M    (const LatticeFermion &in, LatticeFermion &out)
    {
      M_internal(in,out,DaggerNo);
      return norm2(out);
    }
    RealD  PartialFractionFermion5D::Mdag (const LatticeFermion &in, LatticeFermion &out)
    {
      M_internal(in,out,DaggerYes);
      return norm2(out);
    }

    void PartialFractionFermion5D::Meooe       (const LatticeFermion &in, LatticeFermion &out)
    {
      Meooe_internal(in,out,DaggerNo);
    }
    void PartialFractionFermion5D::MeooeDag    (const LatticeFermion &in, LatticeFermion &out)
    {
      Meooe_internal(in,out,DaggerYes);
    }
    void PartialFractionFermion5D::Mooee       (const LatticeFermion &in, LatticeFermion &out)
    {
      Mooee_internal(in,out,DaggerNo);
    }
    void PartialFractionFermion5D::MooeeDag    (const LatticeFermion &in, LatticeFermion &out)
    {
      Mooee_internal(in,out,DaggerYes);
    }

    void PartialFractionFermion5D::MooeeInv    (const LatticeFermion &in, LatticeFermion &out)
    {
      MooeeInv_internal(in,out,DaggerNo);
    }
    void PartialFractionFermion5D::MooeeInvDag (const LatticeFermion &in, LatticeFermion &out)
    {
      MooeeInv_internal(in,out,DaggerYes);
    }

    void  PartialFractionFermion5D::SetCoefficientsTanh(Approx::zolotarev_data *zdata,RealD scale){
      SetCoefficientsZolotarev(1.0/scale,zdata);
    }
    void  PartialFractionFermion5D::SetCoefficientsZolotarev(RealD zolo_hi,Approx::zolotarev_data *zdata){

      // check on degree matching
      //      std::cout << Ls << " Ls"<<std::endl;
      //      std::cout << zdata->n  << " - n"<<std::endl;
      //      std::cout << zdata->da << " -da "<<std::endl;
      //      std::cout << zdata->db << " -db"<<std::endl;
      //      std::cout << zdata->dn << " -dn"<<std::endl;
      //      std::cout << zdata->dd << " -dd"<<std::endl;
      assert(Ls == (2*zdata->da -1) );

      // Part frac
      //      RealD R;
      R=(1+mass)/(1-mass);
      dw_diag = (4.0-M5);

      //      std::vector<RealD> p; 
      //      std::vector<RealD> q;
      p.resize(zdata->da);
      q.resize(zdata->dd);
	
      for(int n=0;n<zdata->da;n++){
	p[n] = zdata -> alpha[n];
      }
      for(int n=0;n<zdata->dd;n++){
	q[n] = -zdata -> ap[n];
      }
      
      scale= part_frac_chroma_convention ? 2.0 : 1.0; // Chroma conventions annoy me

      amax=zolo_hi;
    }

      // Constructors
    PartialFractionFermion5D::PartialFractionFermion5D(LatticeGaugeField &_Umu,
						       GridCartesian         &FiveDimGrid,
						       GridRedBlackCartesian &FiveDimRedBlackGrid,
						       GridCartesian         &FourDimGrid,
						       GridRedBlackCartesian &FourDimRedBlackGrid,
						       RealD _mass,RealD M5) :
      WilsonFermion5D(_Umu,
		      FiveDimGrid, FiveDimRedBlackGrid,
		      FourDimGrid, FourDimRedBlackGrid,M5),
      mass(_mass)

    {
      assert((Ls&0x1)==1); // Odd Ls required
      int nrational=Ls-1;


      Approx::zolotarev_data *zdata = Approx::higham(1.0,nrational);

      // NB: chroma uses a cast to "float" for the zolotarev range(!?).
      // this creates a real difference in the operator which I do not like but we can replicate here
      // to demonstrate compatibility
      //      RealD eps = (zolo_lo / zolo_hi);
      //      zdata = bfm_zolotarev(eps,nrational,0);
      
      SetCoefficientsTanh(zdata,1.0);

      Approx::zolotarev_free(zdata);

    }
 
 }
}
