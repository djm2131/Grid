
static int su2subgroups(GroupName::Sp) { return (nsp * (nsp - 1)) / 2; }

// Sp(2N) has N(2N+1) = 2N^2+N generators
//
// normalise the generators such that
// Trace ( Ta Tb) = 1/2 delta_ab
//
// N generators in the cartan, 2N^2 off
// off diagonal:
//     there are 6 types named a,b,c,d and w,z
//     abcd are N(N-1)/2 each while wz are N each

template <class cplx, ONLY_IF_Sp>
static void generator(int lieIndex, iSp2nMatrix<cplx> &ta, GroupName::Sp) {
  // map lie index into type of generators: diagonal, abcd type, wz type

  int diagIndex;
  int aIndex, bIndex, cIndex, dIndex;
  int wIndex, zIndex;  // a,b,c,d are N(N-1)/2 and w,z are N
  int mod = nsp * (nsp - 1) * 0.5;
  int offdiag =
      2 * nsp * nsp;  // number of generators not in the cartan subalgebra
  int wmod = 4 * mod;
  int zmod = wmod + nsp;
  if (lieIndex >= offdiag) {
    diagIndex = lieIndex - offdiag;  // 0, ... ,N-1
    // std::cout << GridLogMessage << "diag type " << std::endl;
    generatorDiagtype(diagIndex, ta);
    return;
  }
  if ((lieIndex >= wmod) && (lieIndex < zmod)) {
    // std::cout << GridLogMessage << "w type " << std::endl;
    wIndex = lieIndex - wmod;  // 0, ... ,N-1
    generatorWtype(wIndex, ta);
    return;
  }
  if ((lieIndex >= zmod) && (lieIndex < offdiag)) {
    // std::cout << GridLogMessage << "z type " << std::endl;
    // std::cout << GridLogMessage << "lie index " << lieIndex << std::endl;
    // std::cout << GridLogMessage << "z mod " << zmod << std::endl;
    zIndex = lieIndex - zmod;  // 0, ... ,N-1
    generatorZtype(zIndex, ta);
    return;
  }
  if (lieIndex < mod) {  // atype 0, ... , N(N-1)/2=mod
    // std::cout << GridLogMessage << "a type " << std::endl;
    aIndex = lieIndex;
    // std::cout << GridLogMessage << "a indx " << aIndex << std::endl;
    generatorAtype(aIndex, ta);
    return;
  }
  if ((lieIndex >= mod) && lieIndex < 2 * mod) {  // btype mod, ... , 2mod-1
    // std::cout << GridLogMessage << "b type " << std::endl;
    bIndex = lieIndex - mod;
    generatorBtype(bIndex, ta);
    return;
  }
  if ((lieIndex >= 2 * mod) &&
      lieIndex < 3 * mod) {  // ctype 2mod, ... , 3mod-1
    // std::cout << GridLogMessage << "c type " << std::endl;
    cIndex = lieIndex - 2 * mod;
    generatorCtype(cIndex, ta);
    return;
  }
  if ((lieIndex >= 3 * mod) &&
      lieIndex < wmod) {  // ctype 3mod, ... , 4mod-1 = wmod-1
    // std::cout << GridLogMessage << "d type " << std::endl;
    dIndex = lieIndex - 3 * mod;
    generatorDtype(dIndex, ta);
    return;
  }

}  // end of generator

template <class cplx, ONLY_IF_Sp>
static void generatorDiagtype(int diagIndex, iSp2nMatrix<cplx> &ta) {
  // ta(i,i) = - ta(i+N,i+N) = 1/2 for each i index of the cartan subalgebra

  ta = Zero();
  RealD nrm = 1.0 / 2;

  ta()()(diagIndex, diagIndex) = nrm;
  ta()()(diagIndex + nsp, diagIndex + nsp) = -nrm;
}

template <class cplx, ONLY_IF_Sp>
static void generatorAtype(int aIndex, iSp2nMatrix<cplx> &ta) {
  // ta(i,j) = ta(j,i) = -ta(i+N,j+N) = -ta(j+N,i+N) = 1 / 2 sqrt(2)
  // with i<j and i=0,...,N-2
  // follows that j=i+1, ... , N
  int i1, i2;
  ta = Zero();
  RealD nrm = 1 / (2 * std::sqrt(2));

  su2SubGroupIndex(i1, i2, aIndex);
  ta()()(i1, i2) = 1;
  ta()()(i2, i1) = 1;
  ta()()(i1 + nsp, i2 + nsp) = -1;
  ta()()(i2 + nsp, i1 + nsp) = -1;

  ta = ta * nrm;
}

template <class cplx, ONLY_IF_Sp>
static void generatorBtype(int bIndex, iSp2nMatrix<cplx> &ta) {
  // ta(i,j) = -ta(j,i) = ta(i+N,j+N) = -ta(j+N,i+N) = i / 1/ 2 sqrt(2)
  // with i<j and i=0,...,N-2
  // follows that j=i+1, ... , N-1

  int i1, i2;
  ta = Zero();
  cplx i(0.0, 1.0);
  RealD nrm = 1 / (2 * std::sqrt(2));
  su2SubGroupIndex(i1, i2, bIndex);

  ta()()(i1, i2) = i;
  ta()()(i2, i1) = -i;
  ta()()(i1 + nsp, i2 + nsp) = i;
  ta()()(i2 + nsp, i1 + nsp) = -i;

  ta = ta * nrm;
}

template <class cplx, ONLY_IF_Sp>
static void generatorCtype(int cIndex, iSp2nMatrix<cplx> &ta) {
  // ta(i,j+N) = ta(j,i+N) = ta(i+N,j) = ta(j+N,i) = 1 / 2 sqrt(2)

  int i1, i2;
  ta = Zero();
  RealD nrm = 1 / (2 * std::sqrt(2));
  su2SubGroupIndex(i1, i2, cIndex);

  ta()()(i1, i2 + nsp) = 1;
  ta()()(i2, i1 + nsp) = 1;
  ta()()(i1 + nsp, i2) = 1;
  ta()()(i2 + nsp, i1) = 1;

  ta = ta * nrm;
}

template <class cplx, ONLY_IF_Sp>
static void generatorDtype(int dIndex, iSp2nMatrix<cplx> &ta) {
  // ta(i,j+N) = ta(j,i+N) = -ta(i+N,j) = -ta(j+N,i) = i /  2 sqrt(2)

  int i1, i2;
  ta = Zero();
  cplx i(0.0, 1.0);
  RealD nrm = 1 / (2 * std::sqrt(2));
  su2SubGroupIndex(i1, i2, dIndex);

  ta()()(i1, i2 + nsp) = i;
  ta()()(i2, i1 + nsp) = i;
  ta()()(i1 + nsp, i2) = -i;
  ta()()(i2 + nsp, i1) = -i;

  ta = ta * nrm;
}

template <class cplx, ONLY_IF_Sp>
static void generatorWtype(int wIndex, iSp2nMatrix<cplx> &ta) {
  // ta(i,i+N) =  ta(i+N,i) = 1/2

  ta = Zero();
  RealD nrm = 1.0 / 2;  // check

  ta()()(wIndex, wIndex + nsp) = 1;
  ta()()(wIndex + nsp, wIndex) = 1;

  ta = ta * nrm;
}

template <class cplx, ONLY_IF_Sp>
static void generatorZtype(int zIndex, iSp2nMatrix<cplx> &ta) {
  // ta(i,i+N) = - ta(i+N,i) = i/2

  ta = Zero();
  RealD nrm = 1.0 / 2;  // check
  cplx i(0.0, 1.0);
  ta()()(zIndex, zIndex + nsp) = i;
  ta()()(zIndex + nsp, zIndex) = -i;

  ta = ta * nrm;
}

////////////////////////////////////////////////////////////////////////
// Map a su2 subgroup number to the pair of rows that are non zero
////////////////////////////////////////////////////////////////////////
template <ONLY_IF_Sp>
static void su2SubGroupIndex(int &i1, int &i2, int su2_index, GroupName::Sp) {
  assert((su2_index >= 0) && (su2_index < (nsp * (nsp - 1)) / 2));

  int spare = su2_index;
  for (i1 = 0; spare >= (nsp - 1 - i1); i1++) {
    spare = spare - (nsp - 1 - i1);  // remove the Nc-1-i1 terms
  }
  i2 = i1 + 1 + spare;
}

static void testGenerators(GroupName::Sp) {
  Matrix ta;
  Matrix tb;
  std::cout << GridLogMessage
            << "Fundamental - Checking trace ta tb is 0.5 delta_ab "
            << std::endl;
  for (int a = 0; a < AlgebraDimension; a++) {
    for (int b = 0; b < AlgebraDimension; b++) {
      generator(a, ta);
      generator(b, tb);
      Complex tr = TensorRemove(trace(ta * tb));
      std::cout << GridLogMessage << "(" << a << "," << b << ") =  " << tr
                << std::endl;
      if (a == b) assert(abs(tr - Complex(0.5)) < 1.0e-6);
      if (a != b) assert(abs(tr) < 1.0e-6);
    }
  }
  std::cout << GridLogMessage << std::endl;
  std::cout << GridLogMessage << "Fundamental - Checking if hermitian"
            << std::endl;
  for (int a = 0; a < AlgebraDimension; a++) {
    generator(a, ta);
    std::cout << GridLogMessage << a << std::endl;
    assert(norm2(ta - adj(ta)) < 1.0e-6);
  }
  std::cout << GridLogMessage << std::endl;
  std::cout << GridLogMessage << "Fundamental - Checking if traceless"
            << std::endl;
  for (int a = 0; a < AlgebraDimension; a++) {
    generator(a, ta);
    Complex tr = TensorRemove(trace(ta));
    std::cout << GridLogMessage << a << std::endl;
    assert(abs(tr) < 1.0e-6);
  }
}

template <ONLY_IF_Sp>
static void OmegaInvariance(ColourMatrix &in) {
  ColourMatrix Omega;
  Omega = Zero();

  std::cout << GridLogMessage << "I am a ColourMatrix" << std::endl;

  // for (int i = 0; i < ncolour; i++)         wrong?!
  //{
  //     Omega()()(i, 2*ncolour-1-i) = 1.;
  //     Omega()()(2*ncolour-1-i, i) = -1;
  // }
  for (int i = 0; i < nsp; i++) {
    Omega()()(i, nsp + i) = 1.;
    Omega()()(nsp + i, i) = -1;
  }

  auto diff = Omega - (in * Omega * transpose(in));
  auto sdiff = norm2(diff);
  if (norm2(sdiff) < 1e-8) {
    std::cout << GridLogMessage
              << "Symplectic condition satisfied: Omega invariant" << std::endl;
  } else {
    std::cout << GridLogMessage
              << "WARNING!!!!!! Matrix Omega NOT left invariant by "
              << norm2(sdiff) << std::endl;
  }
}

template <typename GaugeField, ONLY_IF_Sp>
static void OmegaInvariance(GaugeField &in) {
  typedef typename GaugeField::vector_type vector_type;
  typedef iSp2nMatrix<vector_type> vMatrixType;
  typedef Lattice<vMatrixType> LatticeMatrixType;

  LatticeMatrixType U(in.Grid());
  LatticeMatrixType OmegaLatt(in.Grid());
  LatticeMatrixType identity(in.Grid());
  RealD vol = in.Grid()->gSites();
  ColourMatrix Omega;

  OmegaLatt = Zero();
  Omega = Zero();
  identity = 1.;

  std::cout << GridLogMessage << "I am a GaugeField " << std::endl;

  U = PeekIndex<LorentzIndex>(in, 1);

  OmegaInvariance(U);
}

template <ONLY_IF_Sp>
static void OmegaInvariance(LatticeColourMatrixD &in) {
  LatticeColourMatrixD OmegaLatt(in.Grid());
  LatticeColourMatrixD identity(in.Grid());
  RealD vol = in.Grid()->gSites();
  ColourMatrix Omega;

  OmegaLatt = Zero();
  Omega = Zero();
  identity = 1.;

  std::cout << GridLogMessage << "I am a LatticeColourMatrix " << std::endl;

  for (int i = 0; i < nsp; i++) {
    Omega()()(i, nsp + i) = 1.;
    Omega()()(nsp + i, i) = -1;
  }

  std::cout << GridLogMessage << "Omega = " << Omega()() << std::endl;
  OmegaLatt = OmegaLatt + (identity * Omega);

  auto diff = OmegaLatt - (in * OmegaLatt * transpose(in));
  auto sdiff = sum(diff);
  // assert( norm2(sdiff) < 1e-8 );
  if (norm2(sdiff) < 1e-8) {
    std::cout << GridLogMessage
              << "Symplectic condition satisfied: Omega invariant" << std::endl;
  } else {
    std::cout << GridLogMessage
              << "WARNING!!!!!! Matrix Omega NOT left invariant by "
              << norm2(sdiff) << std::endl;
  }
}

