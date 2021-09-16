/**
 * @file Hexapod.h
 * @brief Implementation of Hexapod class
 * @author Radek Svec
 * Contact: xsvecr01@stud.fit.vutbr.cz
 */
 
#include "Leg.h"

class Hexapod
{
    public:
        Leg *RF, *RM, *RB, *LF, *LM, *LB;
        Leg* Legs[6];

        uint8_t Strength = 0;
        uint16_t Angle = 0;
        //Position 0 = folded, 1 = standing
        uint8_t Position = 0;
        uint8_t Height = 1;
        uint8_t Gait = 33;
        uint8_t RotL = 0;
        uint8_t RotR = 0;
        
        State state;

        Hexapod(Leg *RightFront, Leg *RightMiddle, Leg *RightBack, Leg *LeftFront, Leg *LeftMiddle, Leg *LeftBack)
        {
            RF = RightFront;
            RM = RightMiddle;
            RB = RightBack;
            LF = LeftFront;
            LM = LeftMiddle;
            LB = LeftBack;
            Legs[0] = RB;
            Legs[1] = RM;
            Legs[2] = RF;
            Legs[3] = LB;
            Legs[4] = LM;
            Legs[5] = LF;
            state = Sitting;
        }

        bool Finished(int items)
        {
            return RF->Finished(items) && RM->Finished(items) && RB->Finished(items) && LF->Finished(items) && LM->Finished(items) && LB->Finished(items);
        }

        void SetHeight()
        {
            for(Leg *l : Legs)
            {
                l->SetXYZ(0, 60, 1, _height, _duration);
            }
        }

        void Adjust()
        {
            for(int i = 0; i < 6; i++)
            {
                Legs[i]->Down(_height, _duration);
                Legs[i]->Sleep((_duration/2) * i);
                AdjustLeg(Legs[i]);
            }
        }

        void Sleep()
        {
            for(Leg *l : Legs)
            {
                l->Sleep(_duration/4);
            }
        }

        void Stop()
        {
            for(Leg *l : Legs)
            {
                l->Clear();
            }
        }

        void Fold(boolean fold)
        {
            if(fold)
            {
                for(Leg *l : Legs)
                {
                    l->SetXYZ(0, 60, _height + 10, _height, _duration/2);
                    l->Fold(_duration/2);
                }
            }
            else
            {
                for(Leg *l : Legs)
                {
                    l->SetXYZ(0, 60, _height + 10, _height, _duration/2);
                    l->SetXYZ(0, 60, 1, _height, _duration/2);
                }
            }
        }

        void PrepStep()
        {
            if(Gait == 33)
                Prep33();
            else if(Gait == 42)
                Prep42(Angle);
            else if(Gait == 51)
                Prep51(Angle);
        }

        void Step()
        {
            if(Gait == 33)
                Step33(Angle);
            else if(Gait == 42)
                Step42(Angle);
            else if(Gait == 51)
                Step51(Angle);
        }

        void PrepRotL()
        {
            if(Gait == 33)
                PrepRot33();
            else if(Gait == 42)
                PrepRot42(false);
            else if(Gait == 51)
                PrepRot51(false);
        }

        void PrepRotR()
        {
            if(Gait == 33)
                PrepRot33();
            else if(Gait == 42)
                PrepRot42();
            else if(Gait == 51)
                PrepRot51();
        }

        void RotateLeft()
        {
            if(Gait == 33)
                Rot33(false);
            else if(Gait == 42)
                Rot42(false);
            else if(Gait == 51)
                Rot51(false);
        }

        void RotateRight()
        {
            if(Gait == 33)
                Rot33();
            else if(Gait == 42)
                Rot42();
            else if(Gait == 51)
                Rot51();
        }

        void ChangeHeight()
        {
            if(Height == 0)
            {
                _height = 15;
                _duration = 600;
            }
            else if(Height == 1)
            {
                _height = 30;
                _duration = 1000;
            }
            else if(Height == 2)
            {
                _height = 50;
                _duration = 1400;
            }
            if(_oldHeight != _height)
            {
                SetHeight();
                _oldHeight = _height;
            }
        }

    private:
        int _height = 30;
        int _oldHeight = _height;
        uint16_t _duration = 1000;


        // dur: _duration * dur
        // 33gait: dur = 0.5
        void RotLeg(Leg *l, float dur, bool right = true)
        {
            float x0 = 0;
            float x1 = 20;
            float x1_ = x1;
            float y = 60;
            float z2 = _height;
            float angle_, angle = 0;

            if(z2 < 20)
            {
                z2 = 20;
            }
            float z1 = z2 / 2;
            float z0 = 1;

            angle = angle_;
            x1 = x1_;
            AngleX1(l, &angle, &x1);

            if(right)
            {
                l->Rotate(72, _duration * dur);
                l->SetXYZ(x1, y, z1, _height, _duration/8);
                l->SetXYZ(x0, y, z2, _height, _duration/8);
                l->SetXYZ(-x1, y, z1, _height, _duration/8);
                l->SetXYZ(-x1, y, z0, _height, _duration/8);
            }
            else
            {
                l->Rotate(108, _duration * dur);
                l->SetXYZ(-x1, y, z1, _height, _duration/8);
                l->SetXYZ(x0, y, z2, _height, _duration/8);
                l->SetXYZ(x1, y, z1, _height, _duration/8);
                l->SetXYZ(x1, y, z0, _height, _duration/8);   
            }
        }


        void AdjustLeg(Leg *l)
        {
            float x0 = 0;
            float y = 60;
            float z2 = _height - 5;
            float z0 = 1;
            
            l->SetXYZ(x0, y, z2, _height, _duration/4);
            l->SetXYZ(x0, y, z0, _height, _duration/4);
        }

        float RotateX(float x, float angle)
        {
            angle = (angle * PI) / 180;
            float res = x * cos(angle);
            return res;
        }
        
        float RotateY(float x, float y, float angle)
        {
            angle = (angle * PI) / 180;
            float res = x * -sin(angle) + y;
            return res;
        }

        void AngleX1(Leg *l, float *angle, float *x1)
        {
            if(!l->_right)
                {
                    *x1 = -*x1;
                    *angle = -*angle;
                }
                
                if(l->_right)
                {
                    if(l->_legPos == Front)
                        *angle -= 45;
                    else if(l->_legPos == Back)
                        *angle += 45;
                }
                else
                {
                    *angle += 180;
                    if(l->_legPos == Front)
                        *angle -= 45;
                    else if(l->_legPos == Back)
                        *angle += 45;
                }
        }

        void Prep33()
        {
            RF->Sleep(_duration/2);
            RB->Sleep(_duration/2);
            LM->Sleep(_duration/2);
        }

        void Step33(float angle)
        {
            float x0 = 0;
            float x1 = 20;
            float x1_ = x1;
            float y = 60;
            float z2 = _height;
            float angle_ = angle;

            
            if(z2 < 20)
            {
                z2 = 20;
            }
            float z1 = z2 / 2;
            float z0 = 1;
            
            for(Leg *l : Legs)
            {
                angle = angle_;
                x1 = x1_;
                AngleX1(l, &angle, &x1);
                                
                l->SetXYZ(RotateX(-x1, angle), RotateY(-x1, y, angle), z0, _height, _duration/4);
                l->SetXYZ(RotateX(-x1, angle), RotateY(-x1, y, angle), z1, _height, _duration/8);
                l->SetXYZ(RotateX(x0, angle),  RotateY(x0, y, angle),  z2, _height, _duration/8);
                l->SetXYZ(RotateX(x1, angle),  RotateY(x1, y, angle),  z1, _height, _duration/8);
                l->SetXYZ(RotateX(x1, angle),  RotateY(x1, y, angle),  z0, _height, _duration/8);
                l->SetXYZ(RotateX(x0, angle),  RotateY(x0, y, angle),  z0, _height, _duration/4);
            }
        }




        void Rot33(bool right = true)
        {
            for(Leg *l : Legs)
            {
                RotLeg(l, 0.5, right);
            }
        }

        void PrepRot33()
        {
            float x0 = 0;
            float x1 = 20;
            float x1_ = x1;
            float y = 60;
            float z2 = _height;
            float z0 = 1;
            
            LF->SetXYZ(x0, y, z2, _height, _duration/4);
            LF->SetXYZ(x0, y, z0, _height, _duration/4);
            LB->SetXYZ(x0, y, z2, _height, _duration/4);
            LB->SetXYZ(x0, y, z0, _height, _duration/4);
            RM->SetXYZ(x0, y, z2, _height, _duration/4);
            RM->SetXYZ(x0, y, z0, _height, _duration/4);
        }

        void PrepRot42(bool right = true)
        {
            float x0 = 0;
            float x1 = 20;
            float x1_ = x1;
            float y = 60;
            float z2 = _height;
            float angle_, angle = 0;

            if(z2 < 20)
            {
                z2 = 20;
            }
            float z1 = z2 / 2;
            float z0 = 1;

            angle = angle_;
            x1 = x1_;
            AngleX1(RF, &angle, &x1);

            if(!right)
            {
                x1 = -x1;
            }

            RF->SetXYZ(x0, y, z2, _height, _duration/4);
            RF->SetXYZ(-x1, y, z1, _height, _duration/8);
            RF->SetXYZ(-x1, y, z0, _height, _duration/8);
            
            angle = angle_;
            x1 = x1_;
            AngleX1(LB, &angle, &x1);

            if(!right)
            {
                x1 = -x1;
            }

            LB->SetXYZ(x0, y, z2, _height, _duration/4);
            LB->SetXYZ(-x1, y, z1, _height, _duration/8);
            LB->SetXYZ(-x1, y, z0, _height, _duration/8);
            

            RotLeg(RM, 0.5, right);
            RotLeg(LM, 0.5, right);

            RotLeg(RB, 1, right);
            RotLeg(LF, 1, right);
        }

        void Rot42(bool right = true)
        {
            for(Leg *l : Legs)
            {
                RotLeg(l, 1, right);
            }
        }

        void Prep42(float angle)
        {
            float x0 = 0;
            float x1 = 20;
            float x1_ = x1;
            float y = 60;
            float z2 = _height;
            float angle_ = angle;
            
            if(z2 < 20)
            {
                z2 = 20;
            }
            float z1 = z2 / 2;
            float z0 = 1;
            
            for(Leg *l : Legs)
            {
                angle = angle_;
                x1 = x1_;
                AngleX1(l, &angle, &x1);
                
                if(l->_legPos == Middle)
                    l->SetXYZ(RotateX(-x1, angle), RotateY(-x1, y, angle), z0, _height, _duration/2);

                if((l->_legPos == Front && !l->_right) || (l->_legPos == Back && l->_right))
                    l->SetXYZ(RotateX(-x1, angle), RotateY(-x1, y, angle), z0, _height, _duration);                
            }
        }

        void Step42(float angle)
        {
            float x0 = 0;
            float x1 = 20;
            float x1_ = x1;
            float y = 60;
            float z2 = _height;
            float angle_ = angle;
            
            if(z2 < 20)
            {
                z2 = 20;
            }
            float z1 = z2 / 2;
            float z0 = 1;
            
            for(Leg *l : Legs)
            {
                angle = angle_;
                x1 = x1_;
                AngleX1(l, &angle, &x1);

                l->SetXYZ(RotateX(-x1, angle), RotateY(-x1, y, angle), z1, _height, _duration/8);
                l->SetXYZ(RotateX(x0, angle),  RotateY(x0, y, angle),  z2, _height, _duration/8);
                l->SetXYZ(RotateX(x1, angle),  RotateY(x1, y, angle),  z1, _height, _duration/8);
                l->SetXYZ(RotateX(x1, angle),  RotateY(x1, y, angle),  z0, _height, _duration/8);
                l->SetXYZ(RotateX(-x1, angle), RotateY(-x1, y, angle), z0, _height, _duration);
            }
        }

        void PrepRot51(bool right = true)
        {
            for(int i = 0; i < 6; i++)
            {
                RotLeg(Legs[i], 0.5 * (i+1), right);
            }
        }

        void Rot51(bool right = true)
        {
            for(Leg *l : Legs)
            {
                RotLeg(l, 2.5, right);
            }
        }

        void Prep51(float angle)
        {
            float x0 = 0;
            float x1 = 20;
            float x1_ = x1;
            float y = 60;
            float z2 = _height;
            float angle_ = angle;
            
            if(z2 < 20)
            {
                z2 = 20;
            }
            float z1 = z2 / 2;
            float z0 = 1;
            
            for(int i = 0; i < 6; i++)
            {
                angle = angle_;
                x1 = x1_;
                AngleX1(Legs[i], &angle, &x1);

                Legs[i]->SetXYZ(RotateX(-x1, angle), RotateY(-x1, y, angle), z0, _height, (_duration / 2) * (i+1));
            }
        }

        void Step51(float angle)
        {
            float x0 = 0;
            float x1 = 20;
            float x1_ = x1;
            float y = 60;
            float z2 = _height;
            float angle_ = angle;

            if(z2 < 20)
            {
                z2 = 20;
            }
            float z1 = z2 / 2;
            float z0 = 1;

            for(Leg *l : Legs)
            {
                angle = angle_;
                x1 = x1_;
                AngleX1(l, &angle, &x1);

                l->SetXYZ(RotateX(-x1, angle), RotateY(-x1, y, angle), z1, _height, _duration/8);
                l->SetXYZ(RotateX(x0, angle),  RotateY(x0, y, angle),  z2, _height, _duration/8);
                l->SetXYZ(RotateX(x1, angle),  RotateY(x1, y, angle),  z1, _height, _duration/8);
                l->SetXYZ(RotateX(x1, angle),  RotateY(x1, y, angle),  z0, _height, _duration/8);
                l->SetXYZ(RotateX(-x1, angle), RotateY(-x1, y, angle), z0, _height, _duration * 2.5);
            }
        }
};
