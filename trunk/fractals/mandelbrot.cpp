#include "mandelbrot.h"
#include "math.h"

template<class T>
Mandelbrot<T>::Mandelbrot(const Transformation<T>& t,
			  const QList< Interpreter<T> >& base,
			  const Interpreter<T>& iteration,
			  int maxIterations,
			  const T& bailout,
			  const T& epsilon,
			  const ColorPalette& bailoutPalette) :
		Rendering<T>(t),
		base_(base),
		iteration_(iteration),
		maxIterations_(maxIterations),
		bailout_(bailout),
		epsilon_(epsilon),
		bailoutPalette_(bailoutPalette) {}

template<class T>
Mandelbrot<T>::~Mandelbrot() {}

template<class T>
void Mandelbrot<T>::color(uchar type, float val,
			  float& r, float& g, float& b, float& a) const {
	if(type == 1) {
		if(val > 2.7183) {
			val /= log(val);
		}

		bailoutPalette_.color(val, r, g, b, a);
	} else {
		r = g = b = 0;
		a = 1;
	}
}

template<class T>
const QList< Interpreter<T> >& Mandelbrot<T>::base() const {
	return base_;
}


template<class T>
QList< Interpreter<T> >& Mandelbrot<T>::base() {
	return base_;
}


template<class T>
const Interpreter<T>& Mandelbrot<T>::iteration() const {
	return iteration_;
}

template<class T>
Interpreter<T>& Mandelbrot<T>::iteration() {
	return iteration_;
}

template<class T>
int Mandelbrot<T>::maxIterations() const {
	return maxIterations_;
}

template<class T>
int& Mandelbrot<T>::maxIterations() {
	return maxIterations_;
}

template<class T>
const T& Mandelbrot<T>::epsilon() const {
	return epsilon_;
}

template<class T>
T& Mandelbrot<T>::epsilon() {
	return epsilon_;
}

template<class T>
const T& Mandelbrot<T>::bailout() const {
	return bailout_;
}

template<class T>
T& Mandelbrot<T>::bailout() {
	return bailout_;
}

template<class T>
const ColorPalette& Mandelbrot<T>::bailoutPalette() const {
	return bailoutPalette_;
}

template<class T>
ColorPalette& Mandelbrot<T>::bailoutPalette() {
	return bailoutPalette_;
}


template<class T>
RenderingGenerator<T>* Mandelbrot<T>::createGenerator(int width, int height) const {
	return new MandelbrotGenerator<T>(width, height, *this);
}

template<class T>
RenderingEnv<T>* Mandelbrot<T>::createEnv() const {
	return new MandelbrotEnv<T>(*this);
}

template<class T>
MandelbrotEnv<T>::MandelbrotEnv(const Mandelbrot<T>& spec) :
	spec_(spec), xs_(new T[10240]), ys_(new T[10240]) {}

template<class T>
MandelbrotEnv<T>::~MandelbrotEnv() {
	delete xs_;
	delete ys_;
}

template<class T>
void MandelbrotEnv<T>::calc(const T& x, const T& y, uchar& type, float& value) {

	int maxIter = spec_.maxIterations();

	T bailoutSqr = spec_.bailout();
	bailoutSqr *= bailoutSqr;

	T epsSqr = spec_.epsilon();
	epsSqr *= epsSqr;

	/* Initialization phase */
	T cr = x;
	T ci = y;

	T zr = 0;
	T zi = 0;

	// Init orbit

	T* xn = xs_;
	T* yn = ys_;

	T nr, ni;

	nr = zr;
	ni = zi;

	// Stack allocation - size hardly has impact

	// Initialization:
	unsigned int ops[16];
	T regs[16];

	for(int i = 0; i < spec_.base().size(); i++) {
		spec_.base()[i].interpret(nr, ni, cr, ci, zr, zi, i, xs_, ys_);

		(*xn) = zr = nr;
		(*yn) = zi = ni;

		xn++;
		yn++;

	}

	int count = spec_.iteration().opCount();

	spec_.iteration().initOps(ops);
	spec_.iteration().initRegs(regs);

	for(int i = spec_.base().size(); i < maxIter; i++) {

		// For the sake of speed avoid pointer resolving and function calls
		// in here. Therefore here using a macro

		for(int j = 0; j < count; j++) {
			unsigned int op = ops[j];
			INTERPRET(op, nr, ni, cr, ci, zr, zi, i, xs_, ys_, regs);
		}

		(*xn) = nr;
		(*yn) = ni;

		xn++;
		yn++;

		// check bailout
		T radSqr = nr * nr + ni * ni;

		if(radSqr > bailoutSqr) {
			// Bailout
			type = 1;

			value = i + 1 + 1 / log(2) * log(log(256) / log(radSqr));
			n_ = i + 1;

			return;
		}

		// Check epsilon
		T dr = zr - nr;
		T di = zi - ni;

		T deltaSqr = dr * dr + di * di;

		if(deltaSqr < epsSqr) {
			// Epsilon limit reached

			n_ = i + 1;

			type = 0;
			value = 0;

			return;
		}

		zr = nr;
		zi = ni;
	}

	// Lake
	value = 0;
	type = 0;

	n_ = maxIter;
}

template<class T>
MandelbrotGenerator<T>::MandelbrotGenerator(int width, int height, const Mandelbrot<T>& spec) :
		RenderingGenerator<T>(width, height, 2, &spec),
		spec_(spec) {
	this->img().setColorProvider(&spec_);
}

template<class T>
MandelbrotGenerator<T>::~MandelbrotGenerator() {}

template<class T>
const Mandelbrot<T>& MandelbrotGenerator<T>::specification() const {
	return spec_;
}

template<class T>
Mandelbrot<T>& MandelbrotGenerator<T>::specification() {
	return spec_;
}

template class Mandelbrot<qreal>;
template class MandelbrotEnv<qreal>;
template class MandelbrotGenerator<qreal>;

template class Mandelbrot<long double>;
template class MandelbrotEnv<long double>;
template class MandelbrotGenerator<long double>;
