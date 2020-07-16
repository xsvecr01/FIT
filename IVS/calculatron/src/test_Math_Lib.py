import unittest
import Math_Lib

class TestMath_Lib(unittest.TestCase):

    def test_add(self):
        testing = Math_Lib.Operations()
        self.assertEqual(testing.m_add(10, 5), 15)
        self.assertEqual(testing.m_add(-1, 1), 0)
        self.assertEqual(testing.m_add(-1, -1), -2)
        self.assertEqual(testing.m_add(10, 50), 60)
        self.assertEqual(testing.m_add(-3, 1), -2)
        self.assertEqual(testing.m_add(-120, -120), -240)
        self.assertEqual(testing.m_add(100, 500), 600)
        self.assertEqual(testing.m_add(-1, -5), -6)
        self.assertEqual(testing.m_add(-1.2, -1.2), -2.4)
        self.assertEqual(testing.m_add(10.123, 5.123), 15.245999999999999)
        self.assertEqual(testing.m_add(-1.5, 1.3), -0.19999999999999996)
        self.assertEqual(testing.m_add(-1, -1), -2)

    def test_subtract(self):
        testing = Math_Lib.Operations()
        self.assertEqual(testing.m_sub(10, 5), 5)
        self.assertEqual(testing.m_sub(-1, 1), -2)
        self.assertEqual(testing.m_sub(-1, -1), 0)
        self.assertEqual(testing.m_sub(-10, -5), -5)
        self.assertEqual(testing.m_sub(-123, 1), -124)
        self.assertEqual(testing.m_sub(-122, -122), 0)
        self.assertEqual(testing.m_sub(-1023, -23165), 22142.0)
        self.assertEqual(testing.m_sub(-651.7, 1.1234), -652.8234)
        self.assertEqual(testing.m_sub(-1.12341234, -1.1523452345), 0.028932894500000028)
        self.assertEqual(testing.m_sub(-3210, -51.2341), -3158.7659)
        self.assertEqual(testing.m_sub(-0.12341234, 0.123451234123), -0.246863574123)
        self.assertEqual(testing.m_sub(-1, -1), 0)

    def test_multiply(self):
        testing = Math_Lib.Operations()
        self.assertEqual(testing.m_mul(10, 5), 50)
        self.assertEqual(testing.m_mul(-1, 1), -1)
        self.assertEqual(testing.m_mul(-1, -1), 1)

        self.assertEqual(testing.m_mul(10, 50), 500)
        self.assertEqual(testing.m_mul(-1.123, 1.123), -1.261129)
        self.assertEqual(testing.m_mul(-0.12341234, -1.1235616), 0.138661366190144)

        self.assertEqual(testing.m_mul(-565110, -455146515), 257207847091650.0)
        self.assertEqual(testing.m_mul(-1.015, 1.123451235), -1.140303003525)
        self.assertEqual(testing.m_mul(-0.0001234, -141234), 17.4282756)

        self.assertEqual(testing.m_mul(123, 123), 15129.0)
        self.assertEqual(testing.m_mul(-345, 345), -119025.0)
        self.assertEqual(testing.m_mul(-1, -1), 1)

    def test_divide(self):
        testing = Math_Lib.Operations()
        self.assertEqual(testing.m_div(-1.1233, 1012031023), -1.1099462115994837e-09)
        self.assertEqual(testing.m_div(-1, -1), 1)
        self.assertEqual(testing.m_div(5, 2), 2.5)
        self.assertEqual(testing.m_div(-1.1233, 101203104423), -1.1099461883154567e-11)
        self.assertEqual(testing.m_div(-133, -1123), 0.11843276936776491)
        self.assertEqual(testing.m_div(5, 3), 1.6666666666666667)
        self.assertEqual(testing.m_div(-1123123.1233234234, 1012031023), -0.0011097714376325234)
        self.assertEqual(testing.m_div(-5, -1235), 0.004048582995951417)
        self.assertEqual(testing.m_div(8, 2), 4)
        self.assertEqual(testing.m_div(-15234523452345.1233, 1012031023), -15053.415464661228)
        self.assertEqual(testing.m_div(-10, -1), 10)
        self.assertEqual(testing.m_div(50, 2), 25)


        #with self.assertRaises(ValueError):
        #    testing = Math_Lib.Operations()
        #    testing.m_div(10, 0)
    
    def test_paw(self):
        testing = Math_Lib.Operations()
        self.assertEqual(testing.m_pow(-1, 1), -1)
        self.assertEqual(testing.m_pow(-1, 0), 1)
        self.assertEqual(testing.m_pow(5, 20), 95367431640625.0)
        self.assertEqual(testing.m_pow(-1, 10), 1)
        self.assertEqual(testing.m_pow(-1, 390), 1)
        self.assertEqual(testing.m_pow(50, 2), 2500)
        self.assertEqual(testing.m_pow(-10, 1), -10)
        self.assertEqual(testing.m_pow(-123, 0), 1)
        self.assertEqual(testing.m_pow(5, 43), 1.1368683772161603e+30)
        self.assertEqual(testing.m_pow(-1123123, 1), -1123123)
        self.assertEqual(testing.m_pow(-1, 5), -1)
        self.assertEqual(testing.m_pow(5, 12), 244140625.0)
    
    def test_root(self):
        testing = Math_Lib.Operations()
        self.assertEqual(testing.m_root(3, 8), 1.147202690439877)
        self.assertEqual(testing.m_root(4, 2), 2.0)
        self.assertEqual(testing.m_root(25, 2), 5.0)
        self.assertEqual(testing.m_root(3, 80), 1.0138273796123731)
        self.assertEqual(testing.m_root(4, 20), 1.0717734625362931)
        self.assertEqual(testing.m_root(25, 20), 1.174618943088019)
        self.assertEqual(testing.m_root(3, 800), 1.0013742087214894)
        self.assertEqual(testing.m_root(4, 200), 1.0069555500567189)
        self.assertEqual(testing.m_root(25, 200), 1.0162245912673256)
        self.assertEqual(testing.m_root(3, 8000), 1.000137335965804)
        self.assertEqual(testing.m_root(4, 2000), 1.0006933874625807)
        self.assertEqual(testing.m_root(25, 2000), 1.0016107337527294)

    def test_log(self):
        testing = Math_Lib.Operations()
        self.assertEqual(testing.m_log(3), 1.0986122886681098)
        self.assertEqual(testing.m_log(4), 1.3862943611198906)
        self.assertEqual(testing.m_log(2), 0.6931471805599453)
        self.assertEqual(testing.m_log(5), 1.6094379124341003)
        self.assertEqual(testing.m_log(6), 1.791759469228055)
        self.assertEqual(testing.m_log(7), 1.9459101490553132)
        self.assertEqual(testing.m_log(8), 2.0794415416798357)
        self.assertEqual(testing.m_log(9), 2.1972245773362196)
        self.assertEqual(testing.m_log(10), 2.302585092994046)
        self.assertEqual(testing.m_log(11), 2.3978952727983707)
        self.assertEqual(testing.m_log(12), 2.4849066497880004)
        self.assertEqual(testing.m_log(13), 2.5649493574615367)

    def test_fact(self):
        testing = Math_Lib.Operations()
        self.assertEqual(testing.m_fact(3), 6)
        self.assertEqual(testing.m_fact(4), 24)
        self.assertEqual(testing.m_fact(5), 120)
        self.assertEqual(testing.m_fact(6), 720)
        self.assertEqual(testing.m_fact(7), 5040)
        self.assertEqual(testing.m_fact(8), 40320)
        self.assertEqual(testing.m_fact(9), 362880)
        self.assertEqual(testing.m_fact(10), 3628800)
        self.assertEqual(testing.m_fact(11), 39916800)
        self.assertEqual(testing.m_fact(12), 479001600)
        self.assertEqual(testing.m_fact(13), 6227020800)
        self.assertEqual(testing.m_fact(14), 87178291200)