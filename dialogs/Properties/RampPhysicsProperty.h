#ifndef H_RAMP_PHYSICS_PROPERTY
#define H_RAMP_PHYSICS_PROPERTY

class RampPhysicsProperty: public BasePropertyDialog
{
public:
    RampPhysicsProperty(VectorProtected<ISelect> *pvsel);
    virtual ~RampPhysicsProperty()
    {
    }

    void UpdateVisuals(const int dispid=-1);
    void UpdateProperties(const int dispid);

protected:
    virtual BOOL OnInitDialog();

private:
    CComboBox m_physicsMaterialCombo;
    EditBox   m_hitThresholdEdit;
    EditBox   m_leftWallEdit;
    EditBox   m_rightWallEdit;
    EditBox   m_elasticityEdit;
    EditBox   m_frictionEdit;
    EditBox   m_scatterAngleEdit;
};

#endif 
