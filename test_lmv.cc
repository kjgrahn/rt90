/* The reference data at LMV must convert well.
 *
 * Samband och parametrar
 * Lantmäteriet I-Divisionen Geodesi
 * 2006-05-03
 * <http://www.lantmateriet.se/upload/filer/kartor/
 * geodesi_gps_och_detaljmatning/Transformationer/
 * SWEREF99_RT90_Samband/kontrollpunkter.pdf>
 * 
 */
#include <testicle.h>

#include "planar.h"

void assert_from(const Planar& a, const Planar& b);
void assert_to(const Planar& a, const Planar& b);

namespace {
    namespace rt90 {
	const Planar A(7453389.762, 1727060.905);
	const Planar B(7047738.415, 1522128.637);
	const Planar C(6671665.273, 1441843.186);
	const Planar D(6249111.351, 1380573.079);
    }
    namespace sweref99 {
	const Planar A(7454204.638, 761811.242);
	const Planar B(7046077.605, 562140.337);
	const Planar C(6669189.376, 486557.055);
	const Planar D(6246136.458, 430374.835);
    }
}


namespace lmv {

    void test_forward()
    {
	assert_from(rt90::A, sweref99::A);
	assert_from(rt90::B, sweref99::B);
	assert_from(rt90::C, sweref99::C);
	assert_from(rt90::D, sweref99::D);
    }

    void test_backward()
    {
	assert_to(sweref99::A, rt90::A);
	assert_to(sweref99::B, rt90::B);
	assert_to(sweref99::C, rt90::C);
	assert_to(sweref99::D, rt90::D);
    }
}
