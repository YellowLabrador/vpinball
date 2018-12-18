#include "StdAfx.h"
#include "meshes/flipperBase.h"
#include "objloader.h"

static const float vertsTipBottomf[13 * 3] =
{
   -0.101425f, 0.786319f, 0.003753f,
   -0.097969f, 0.812569f, 0.003753f,
   -0.087837f, 0.837031f, 0.003753f,
   -0.071718f, 0.858037f, 0.003753f,
   -0.050713f, 0.874155f, 0.003753f,
   -0.026251f, 0.884288f, 0.003753f,
   -0.000000f, 0.887744f, 0.003753f,
   0.026251f, 0.884288f, 0.003753f,
   0.050713f, 0.874155f, 0.003753f,
   0.071718f, 0.858037f, 0.003753f,
   0.087837f, 0.837031f, 0.003753f,
   0.097969f, 0.812569f, 0.003753f,
   0.101425f, 0.786319f, 0.003753f
};

static const Vertex3Ds* const vertsTipBottom = (Vertex3Ds*)vertsTipBottomf;

static const float vertsTipTopf[13 * 3] =
{
   -0.101425f, 0.786319f, 1.004253f,
   -0.097969f, 0.812569f, 1.004253f,
   -0.087837f, 0.837031f, 1.004253f,
   -0.071718f, 0.858037f, 1.004253f,
   -0.050713f, 0.874155f, 1.004253f,
   -0.026251f, 0.884288f, 1.004253f,
   -0.000000f, 0.887744f, 1.004253f,
   0.026251f, 0.884288f, 1.004253f,
   0.050713f, 0.874155f, 1.004253f,
   0.071718f, 0.858037f, 1.004253f,
   0.087837f, 0.837031f, 1.004253f,
   0.097969f, 0.812569f, 1.004253f,
   0.101425f, 0.786319f, 1.004253f
};

static const Vertex3Ds* const vertsTipTop = (Vertex3Ds*)vertsTipTopf;

static const float vertsBaseBottomf[13 * 3] =
{
   -0.100762f, -0.000000f, 0.003753f,
   -0.097329f, -0.026079f, 0.003753f,
   -0.087263f, -0.050381f, 0.003753f,
   -0.071250f, -0.071250f, 0.003753f,
   -0.050381f, -0.087263f, 0.003753f,
   -0.026079f, -0.097329f, 0.003753f,
   -0.000000f, -0.100762f, 0.003753f,
   0.026079f, -0.097329f, 0.003753f,
   0.050381f, -0.087263f, 0.003753f,
   0.071250f, -0.071250f, 0.003753f,
   0.087263f, -0.050381f, 0.003753f,
   0.097329f, -0.026079f, 0.003753f,
   0.100762f, -0.000000f, 0.003753f
};

static const Vertex3Ds* const vertsBaseBottom = (Vertex3Ds*)vertsBaseBottomf;

static const float vertsBaseTopf[13 * 3] =
{
   -0.100762f, 0.000000f, 1.004253f,
   -0.097329f, -0.026079f, 1.004253f,
   -0.087263f, -0.050381f, 1.004253f,
   -0.071250f, -0.071250f, 1.004253f,
   -0.050381f, -0.087263f, 1.004253f,
   -0.026079f, -0.097329f, 1.004253f,
   -0.000000f, -0.100762f, 1.004253f,
   0.026079f, -0.097329f, 1.004253f,
   0.050381f, -0.087263f, 1.004253f,
   0.071250f, -0.071250f, 1.004253f,
   0.087263f, -0.050381f, 1.004253f,
   0.097329f, -0.026079f, 1.004253f,
   0.100762f, -0.000000f, 1.004253f
};

static const Vertex3Ds* const vertsBaseTop = (Vertex3Ds*)vertsBaseTopf;

Flipper::Flipper()
{
   m_phitflipper = NULL;
   vertexBuffer = NULL;
   indexBuffer = NULL;
   m_ptable = NULL;
   memset(m_d.m_szImage, 0, MAXTOKEN);
}

Flipper::~Flipper()
{
   if (vertexBuffer)
      vertexBuffer->release();
   if (indexBuffer)
      indexBuffer->release();
}

void Flipper::UpdateUnitsInfo()
{
}

HRESULT Flipper::Init(PinTable *ptable, float x, float y, bool fromMouseClick)
{
   m_ptable = ptable;

   m_d.m_Center.x = x;
   m_d.m_Center.y = y;

   SetDefaults(fromMouseClick);

   m_phitflipper = NULL;

   return InitVBA(fTrue, 0, NULL);
}

void Flipper::SetDefaults(bool fromMouseClick)
{
   static const char regKey[] = "DefaultProps\\Flipper";

   HRESULT hr;
   float fTmp;
   int iTmp;

   SetDefaultPhysics(fromMouseClick);

   hr = GetRegStringAsFloat(regKey, "StartAngle", &fTmp);
   m_d.m_StartAngle = (hr == S_OK) && fromMouseClick ? fTmp : 121.f;

   hr = GetRegStringAsFloat(regKey, "EndAngle", &fTmp);
   m_d.m_EndAngle = (hr == S_OK) && fromMouseClick ? fTmp : 70.f;

   hr = GetRegStringAsFloat(regKey, "BaseRadius", &fTmp);
   m_d.m_BaseRadius = (hr == S_OK) && fromMouseClick ? fTmp : 21.5f;        // 15

   hr = GetRegStringAsFloat(regKey, "EndRadius", &fTmp);
   m_d.m_EndRadius = (hr == S_OK) && fromMouseClick ? fTmp : 13.f;          // 6

   hr = GetRegStringAsFloat(regKey, "Length", &fTmp);
   m_d.m_FlipperRadiusMax = (hr == S_OK) && fromMouseClick ? fTmp : 130.f;  // 80

   hr = GetRegStringAsFloat(regKey, "MaxDifLength", &fTmp);
   m_d.m_FlipperRadiusMin = (hr == S_OK) && fromMouseClick ? fTmp : 0.f;

   m_d.m_FlipperRadius = m_d.m_FlipperRadiusMax;

   hr = GetRegInt(regKey, "TimerEnabled", &iTmp);
   if ((hr == S_OK) && fromMouseClick)
      m_d.m_tdr.m_fTimerEnabled = iTmp == 0 ? false : true;
   else
      m_d.m_tdr.m_fTimerEnabled = false;

   hr = GetRegInt(regKey, "TimerInterval", &iTmp);
   m_d.m_tdr.m_TimerInterval = (hr == S_OK) && fromMouseClick ? iTmp : 100;

   hr = GetRegInt(regKey, "Color", &iTmp);
   m_d.m_color = (hr == S_OK) && fromMouseClick ? iTmp : RGB(255, 255, 255);

   hr = GetRegInt(regKey, "RubberColor", &iTmp);
   m_d.m_rubbercolor = (hr == S_OK) && fromMouseClick ? iTmp : RGB(128, 50, 50);

   hr = GetRegString(regKey, "Surface", &m_d.m_szSurface, MAXTOKEN);
   if ((hr != S_OK) || !fromMouseClick)
      m_d.m_szSurface[0] = 0;

   hr = GetRegStringAsFloat(regKey, "Height", &fTmp);
   m_d.m_height = (hr == S_OK) && fromMouseClick ? fTmp : 50.f;

   hr = GetRegStringAsFloat(regKey, "RubberThickness", &fTmp);
   m_d.m_rubberthickness = (hr == S_OK) && fromMouseClick ? fTmp : 7.f;

   hr = GetRegStringAsFloat(regKey, "RubberHeight", &fTmp);
   m_d.m_rubberheight = (hr == S_OK) && fromMouseClick ? fTmp : 19.f;

   hr = GetRegStringAsFloat(regKey, "RubberWidth", &fTmp);
   m_d.m_rubberwidth = (hr == S_OK) && fromMouseClick ? fTmp : 24.f;

   hr = GetRegInt(regKey, "Visible", &iTmp);
   if ((hr == S_OK) && fromMouseClick)
      m_d.m_fVisible = iTmp == 0 ? false : true;
   else
      m_d.m_fVisible = true;

   hr = GetRegInt(regKey, "Enabled", &iTmp);
   if ((hr == S_OK) && fromMouseClick)
      m_d.m_fEnabled = iTmp == 0 ? false : true;
   else
      m_d.m_fEnabled = true;

   hr = GetRegInt(regKey, "ReflectionEnabled", &iTmp);
   if ((hr == S_OK) && fromMouseClick)
      m_d.m_fReflectionEnabled = iTmp == 0 ? false : true;
   else
      m_d.m_fReflectionEnabled = true;
}

void Flipper::WriteRegDefaults()
{
   static const char regKey[] = "DefaultProps\\Flipper";

   SetRegValueFloat(regKey, "Scatter", m_d.m_scatter);
   SetRegValueFloat(regKey, "Strength", m_d.m_strength);
   SetRegValueFloat(regKey, "EOSTorque", m_d.m_torqueDamping);
   SetRegValueFloat(regKey, "EOSTorqueAngle", m_d.m_torqueDampingAngle);
   SetRegValueFloat(regKey, "StartAngle", m_d.m_StartAngle);
   SetRegValueFloat(regKey, "EndAngle", m_d.m_EndAngle);
   SetRegValueFloat(regKey, "BaseRadius", m_d.m_BaseRadius);
   SetRegValueFloat(regKey, "EndRadius", m_d.m_EndRadius);
   SetRegValueFloat(regKey, "MaxDifLength", m_d.m_FlipperRadiusMin);
   SetRegValueFloat(regKey, "ReturnStrength", m_d.m_return);
   SetRegValueFloat(regKey, "Length", m_d.m_FlipperRadiusMax);
   SetRegValueFloat(regKey, "Mass", m_d.m_mass);
   SetRegValueFloat(regKey, "Elasticity", m_d.m_elasticity);
   SetRegValueFloat(regKey, "ElasticityFalloff", m_d.m_elasticityFalloff);
   SetRegValueFloat(regKey, "Friction", m_d.m_friction);
   SetRegValueFloat(regKey, "RampUp", m_d.m_rampUp);
   SetRegValueBool(regKey, "TimerEnabled", m_d.m_tdr.m_fTimerEnabled);
   SetRegValueInt(regKey, "TimerInterval", m_d.m_tdr.m_TimerInterval);
   SetRegValueInt(regKey, "Color", m_d.m_color);
   SetRegValueInt(regKey, "RubberColor", m_d.m_rubbercolor);
   SetRegValue(regKey, "Surface", REG_SZ, &m_d.m_szSurface, (DWORD)strlen(m_d.m_szSurface));
   SetRegValueFloat(regKey, "Height", m_d.m_height);
   SetRegValueFloat(regKey, "RubberThickness", m_d.m_rubberthickness);
   SetRegValueFloat(regKey, "RubberHeight", m_d.m_rubberheight);
   SetRegValueFloat(regKey, "RubberWidth", m_d.m_rubberwidth);
   SetRegValueBool(regKey, "Visible", m_d.m_fVisible);
   SetRegValueBool(regKey, "Enabled", m_d.m_fEnabled);
   SetRegValueBool(regKey, "ReflectionEnabled", m_d.m_fReflectionEnabled);
}


void Flipper::GetTimers(vector<HitTimer*> &pvht)
{
   IEditable::BeginPlay();

   HitTimer * const pht = new HitTimer();
   pht->m_interval = m_d.m_tdr.m_TimerInterval >= 0 ? max(m_d.m_tdr.m_TimerInterval, MAX_TIMER_MSEC_INTERVAL) : -1;
   pht->m_nextfire = pht->m_interval;
   pht->m_pfe = (IFireEvents *)this;

   m_phittimer = pht;

   if (m_d.m_tdr.m_fTimerEnabled)
      pvht.push_back(pht);
}

void Flipper::UpdatePhysicsSettings()
{
   if (m_d.m_OverridePhysics || (m_ptable->m_overridePhysicsFlipper && m_ptable->m_overridePhysics))
   {
      const int idx = m_d.m_OverridePhysics ? (m_d.m_OverridePhysics-1) : (m_ptable->m_overridePhysics-1);
      char tmp[256];

      m_d.m_OverrideMass = 1.f;
      sprintf_s(tmp, 256, "FlipperPhysicsMass%d", idx);
      HRESULT hr = GetRegStringAsFloat("Player", tmp, &m_d.m_OverrideMass);
      if (hr != S_OK)
         m_d.m_OverrideMass = 1.f;

      if (m_d.m_OverrideMass < 0.0f)
         m_d.m_OverrideMass = m_d.m_mass;

      m_d.m_OverrideStrength = 2200.f;
      sprintf_s(tmp, 256, "FlipperPhysicsStrength%d", idx);
      hr = GetRegStringAsFloat("Player", tmp, &m_d.m_OverrideStrength);
      if (hr != S_OK)
         m_d.m_OverrideStrength = 2200.f;
      
      if (m_d.m_OverrideStrength < 0.0f)
         m_d.m_OverrideStrength = m_d.m_strength;

      m_d.m_OverrideElasticity = 0.8f;
      sprintf_s(tmp, 256, "FlipperPhysicsElasticity%d", idx);
      hr = GetRegStringAsFloat("Player", tmp, &m_d.m_OverrideElasticity);
      if (hr != S_OK)
         m_d.m_OverrideElasticity = 0.8f;

      if (m_d.m_OverrideElasticity < 0.0f)
         m_d.m_OverrideElasticity = m_d.m_elasticity;

      m_d.m_OverrideScatterAngle = 0.f;
      sprintf_s(tmp, 256, "FlipperPhysicsScatter%d", idx);
      hr = GetRegStringAsFloat("Player", tmp, &m_d.m_OverrideScatterAngle);
      if (hr != S_OK)
         m_d.m_OverrideScatterAngle = 0.f;
     
     if (m_d.m_OverrideScatterAngle < 0.0f)
        m_d.m_OverrideScatterAngle = m_d.m_scatter;

     m_d.m_OverrideReturnStrength = 0.058f;
     sprintf_s(tmp, 256, "FlipperPhysicsReturnStrength%d", idx);
     hr = GetRegStringAsFloat("Player", tmp, &m_d.m_OverrideReturnStrength);
     if (hr != S_OK)
        m_d.m_OverrideReturnStrength = 0.058f;

     if (m_d.m_OverrideReturnStrength < 0.0f)
        m_d.m_OverrideReturnStrength = m_d.m_return;

      m_d.m_OverrideElasticityFalloff = 0.43f;
      sprintf_s(tmp, 256, "FlipperPhysicsElasticityFalloff%d", idx);
      hr = GetRegStringAsFloat("Player", tmp, &m_d.m_OverrideElasticityFalloff);
      if (hr != S_OK)
         m_d.m_OverrideElasticityFalloff = 0.43f;

      if (m_d.m_OverrideElasticityFalloff < 0.0f)
         m_d.m_OverrideElasticityFalloff = m_d.m_elasticityFalloff;

      m_d.m_OverrideFriction = 0.6f;
      sprintf_s(tmp, 256, "FlipperPhysicsFriction%d", idx);
      hr = GetRegStringAsFloat("Player", tmp, &m_d.m_OverrideFriction);
      if (hr != S_OK)
         m_d.m_OverrideFriction = 0.6f;

      if (m_d.m_OverrideFriction < 0.0f)
         m_d.m_OverrideFriction = m_d.m_friction;

      m_d.m_OverrideCoilRampUp = 3.f;
      sprintf_s(tmp, 256, "FlipperPhysicsCoilRampUp%d", idx);
      hr = GetRegStringAsFloat("Player", tmp, &m_d.m_OverrideCoilRampUp);
      if (hr != S_OK)
         m_d.m_OverrideCoilRampUp = 3.f;

      if (m_d.m_OverrideCoilRampUp < 0.0f)
         m_d.m_OverrideCoilRampUp = m_d.m_rampUp;

      m_d.m_OverrideTorqueDamping = 0.75f;
      sprintf_s(tmp, 256, "FlipperPhysicsEOSTorque%d", idx);
      hr = GetRegStringAsFloat("Player", tmp, &m_d.m_OverrideTorqueDamping);
      if (hr != S_OK)
         m_d.m_OverrideTorqueDamping = 0.75f;

      if (m_d.m_OverrideTorqueDamping < 0.0f)
         m_d.m_OverrideTorqueDamping = m_d.m_torqueDamping;

      m_d.m_OverrideTorqueDampingAngle = 6.f;
      sprintf_s(tmp, 256, "FlipperPhysicsEOSTorqueAngle%d", idx);
      hr = GetRegStringAsFloat("Player", tmp, &m_d.m_OverrideTorqueDampingAngle);
      if (hr != S_OK)
         m_d.m_OverrideTorqueDampingAngle = 6.f;

      if (m_d.m_OverrideTorqueDampingAngle < 0.0f)
         m_d.m_OverrideTorqueDampingAngle = m_d.m_torqueDampingAngle;
   }
}

void Flipper::GetHitShapes(vector<HitObject*> &pvho)
{
   UpdatePhysicsSettings();

   //

   const float height = m_ptable->GetSurfaceHeight(m_d.m_szSurface, m_d.m_Center.x, m_d.m_Center.y);

   if (m_d.m_FlipperRadiusMin > 0.f && m_d.m_FlipperRadiusMax > m_d.m_FlipperRadiusMin)
   {
      m_d.m_FlipperRadius = m_d.m_FlipperRadiusMax - (m_d.m_FlipperRadiusMax - m_d.m_FlipperRadiusMin) * m_ptable->m_globalDifficulty;
      m_d.m_FlipperRadius = max(m_d.m_FlipperRadius, m_d.m_BaseRadius - m_d.m_EndRadius + 0.05f);
   }
   else m_d.m_FlipperRadius = m_d.m_FlipperRadiusMax;

   HitFlipper * const phf = new HitFlipper(m_d.m_Center, max(m_d.m_BaseRadius, 0.01f), max(m_d.m_EndRadius, 0.01f),
      max(m_d.m_FlipperRadius, 0.01f), ANGTORAD(m_d.m_StartAngle), ANGTORAD(m_d.m_EndAngle), height, height + m_d.m_height, this);

   phf->m_flipperMover.m_fEnabled = m_d.m_fEnabled;
   phf->m_flipperMover.m_fVisible = m_d.m_fVisible;

   pvho.push_back(phf);

   m_phitflipper = phf;
}

void Flipper::GetHitShapesDebug(vector<HitObject*> &pvho)
{
   const float height = m_ptable->GetSurfaceHeight(m_d.m_szSurface, m_d.m_Center.x, m_d.m_Center.y);

   if (m_d.m_FlipperRadiusMin > 0.f && m_d.m_FlipperRadiusMax > m_d.m_FlipperRadiusMin)
   {
      m_d.m_FlipperRadius = m_d.m_FlipperRadiusMax - (m_d.m_FlipperRadiusMax - m_d.m_FlipperRadiusMin) * m_ptable->m_globalDifficulty;
      m_d.m_FlipperRadius = max(m_d.m_FlipperRadius, m_d.m_BaseRadius - m_d.m_EndRadius + 0.05f);
   }
   else m_d.m_FlipperRadius = m_d.m_FlipperRadiusMax;

   Hit3DPoly * const pcircle = new Hit3DPoly(m_d.m_Center.x, m_d.m_Center.y, height + m_d.m_height, m_d.m_FlipperRadius + m_d.m_EndRadius, 32);
   pvho.push_back(pcircle);
}

void Flipper::EndPlay()
{
   if (m_phitflipper) // Failed player case
   {
      m_phitflipper = NULL;
   }
   if (vertexBuffer)
   {
      vertexBuffer->release();
      vertexBuffer = NULL;
   }
   if (indexBuffer)
   {
      indexBuffer->release();
      indexBuffer = NULL;
   }

   IEditable::EndPlay();
}

void Flipper::SetVertices(const float basex, const float basey, const float angle, Vertex2D * const pvEndCenter, Vertex2D * const rgvTangents, const float baseradius, const float endradius) const
{
   const float fradius = m_d.m_FlipperRadius;
   const float fa = asinf((baseradius - endradius) / fradius); //face to centerline angle (center to center)

   const float faceNormOffset = (float)(M_PI / 2.0) - fa; //angle of normal when flipper center line at angle zero	

   const float endx = basex + fradius*sinf(angle); //place end radius center
   pvEndCenter->x = endx;
   const float endy = basey - fradius*cosf(angle);
   pvEndCenter->y = endy;

   const float faceNormx1 =  sinf(angle - faceNormOffset); // normals to new face positions
   const float faceNormy1 = -cosf(angle - faceNormOffset);
   const float faceNormx2 =  sinf(angle + faceNormOffset); // both faces
   const float faceNormy2 = -cosf(angle + faceNormOffset);

   rgvTangents[0].x = basex + baseradius*faceNormx1;	// endpoints of faces
   rgvTangents[0].y = basey + baseradius*faceNormy1;

   rgvTangents[1].x = endx + endradius*faceNormx1;
   rgvTangents[1].y = endy + endradius*faceNormy1;

   rgvTangents[3].x = basex + baseradius*faceNormx2;
   rgvTangents[3].y = basey + baseradius*faceNormy2;

   rgvTangents[2].x = endx + endradius*faceNormx2;
   rgvTangents[2].y = endy + endradius*faceNormy2;
}

void Flipper::UIRenderPass1(Sur * const psur)
{
   const float rubBaseRadius = m_d.m_BaseRadius - m_d.m_rubberthickness;
   const float rubEndRadius = m_d.m_EndRadius - m_d.m_rubberthickness;
   const float anglerad = ANGTORAD(m_d.m_StartAngle);
   //const float anglerad2 = ANGTORAD(m_d.m_EndAngle);

   m_d.m_FlipperRadius = m_d.m_FlipperRadiusMax;

   psur->SetFillColor(m_ptable->RenderSolid() ? g_pvp->m_fillColor : -1);
   psur->SetBorderColor(-1, false, 0);
   psur->SetLineColor(RGB(0, 0, 0), false, 0);

   Vertex2D vendcenter;
   Vertex2D rgv[4];
   SetVertices(m_d.m_Center.x, m_d.m_Center.y, anglerad, &vendcenter, rgv, m_d.m_BaseRadius, m_d.m_EndRadius);

   psur->SetObject(this);

   psur->Polygon(rgv, 4);
   psur->Ellipse(m_d.m_Center.x, m_d.m_Center.y, m_d.m_BaseRadius);
   psur->Ellipse(vendcenter.x, vendcenter.y, m_d.m_EndRadius);

   // rubber
   SetVertices(m_d.m_Center.x, m_d.m_Center.y, anglerad, &vendcenter, rgv, rubBaseRadius, rubEndRadius);

   psur->SetObject(this);
   psur->SetLineColor(RGB(128, 0, 0), false, 0);

   psur->Polygon(rgv, 4);
   psur->Ellipse(m_d.m_Center.x, m_d.m_Center.y, rubBaseRadius);
   psur->Ellipse(vendcenter.x, vendcenter.y, rubEndRadius);
}

void Flipper::UIRenderPass2(Sur * const psur)
{
   const float anglerad = ANGTORAD(m_d.m_StartAngle);
   const float anglerad2 = ANGTORAD(m_d.m_EndAngle);
   const float rubBaseRadius = m_d.m_BaseRadius - m_d.m_rubberthickness;
   const float rubEndRadius = m_d.m_EndRadius - m_d.m_rubberthickness;

   Vertex2D vendcenter;
   Vertex2D rgv[4];
   SetVertices(m_d.m_Center.x, m_d.m_Center.y, anglerad, &vendcenter, rgv, m_d.m_BaseRadius, m_d.m_EndRadius);

   psur->SetFillColor(m_ptable->RenderSolid() ? g_pvp->m_fillColor : -1);
   psur->SetBorderColor(-1, false, 0);
   psur->SetLineColor(RGB(0, 0, 0), false, 0);

   psur->SetObject(this);

   psur->Line(rgv[0].x, rgv[0].y, rgv[1].x, rgv[1].y);
   psur->Line(rgv[2].x, rgv[2].y, rgv[3].x, rgv[3].y);

   psur->Arc(m_d.m_Center.x, m_d.m_Center.y, m_d.m_BaseRadius, rgv[0].x, rgv[0].y, rgv[3].x, rgv[3].y);
   psur->Arc(vendcenter.x, vendcenter.y, m_d.m_EndRadius, rgv[2].x, rgv[2].y, rgv[1].x, rgv[1].y);

   //rubber
   SetVertices(m_d.m_Center.x, m_d.m_Center.y, anglerad, &vendcenter, rgv, rubBaseRadius, rubEndRadius);

   psur->SetFillColor(m_ptable->RenderSolid() ? g_pvp->m_fillColor : -1);
   psur->SetBorderColor(-1, false, 0);
   psur->SetLineColor(RGB(0, 0, 0), false, 0);

   psur->SetObject(this);

   psur->Line(rgv[0].x, rgv[0].y, rgv[1].x, rgv[1].y);
   psur->Line(rgv[2].x, rgv[2].y, rgv[3].x, rgv[3].y);

   psur->Arc(m_d.m_Center.x, m_d.m_Center.y, rubBaseRadius, rgv[0].x, rgv[0].y, rgv[3].x, rgv[3].y);
   psur->Arc(vendcenter.x, vendcenter.y, rubEndRadius, rgv[2].x, rgv[2].y, rgv[1].x, rgv[1].y);


   //draw the flipper up position
   SetVertices(m_d.m_Center.x, m_d.m_Center.y, anglerad2, &vendcenter, rgv, m_d.m_BaseRadius, m_d.m_EndRadius);

   psur->SetLineColor(RGB(128, 128, 128), true, 0);

   psur->Line(rgv[0].x, rgv[0].y, rgv[1].x, rgv[1].y);
   psur->Line(rgv[2].x, rgv[2].y, rgv[3].x, rgv[3].y);

   psur->Arc(m_d.m_Center.x, m_d.m_Center.y, m_d.m_BaseRadius, rgv[0].x, rgv[0].y, rgv[3].x, rgv[3].y);
   psur->Arc(vendcenter.x, vendcenter.y, m_d.m_EndRadius, rgv[2].x, rgv[2].y, rgv[1].x, rgv[1].y);

   rgv[0].x = m_d.m_Center.x + sinf(anglerad) * (m_d.m_FlipperRadius + m_d.m_EndRadius);
   rgv[0].y = m_d.m_Center.y - cosf(anglerad) * (m_d.m_FlipperRadius + m_d.m_EndRadius);

   rgv[1].x = m_d.m_Center.x + sinf(anglerad2) * (m_d.m_FlipperRadius + m_d.m_EndRadius);
   rgv[1].y = m_d.m_Center.y - cosf(anglerad2) * (m_d.m_FlipperRadius + m_d.m_EndRadius);

   if (m_d.m_EndAngle < m_d.m_StartAngle)
      psur->Arc(m_d.m_Center.x, m_d.m_Center.y, m_d.m_FlipperRadius + m_d.m_EndRadius
      , rgv[0].x, rgv[0].y, rgv[1].x, rgv[1].y);
   else psur->Arc(m_d.m_Center.x, m_d.m_Center.y, m_d.m_FlipperRadius + m_d.m_EndRadius
      , rgv[1].x, rgv[1].y, rgv[0].x, rgv[0].y);

   if (m_d.m_FlipperRadiusMin > 0.f && m_d.m_FlipperRadiusMax > m_d.m_FlipperRadiusMin)
   {
      m_d.m_FlipperRadius = (m_ptable->m_globalDifficulty > 0.f) ? m_d.m_FlipperRadiusMin : m_d.m_FlipperRadiusMax;
      m_d.m_FlipperRadius = max(m_d.m_FlipperRadius, m_d.m_BaseRadius - m_d.m_EndRadius + 0.05f);
   }
   else return;

   if (m_d.m_FlipperRadius != m_d.m_FlipperRadiusMax)
   {
      SetVertices(m_d.m_Center.x, m_d.m_Center.y, anglerad, &vendcenter, rgv, m_d.m_BaseRadius, m_d.m_EndRadius);

      psur->SetObject(this);

      psur->Line(rgv[0].x, rgv[0].y, rgv[1].x, rgv[1].y);
      psur->Line(rgv[2].x, rgv[2].y, rgv[3].x, rgv[3].y);

      psur->Arc(m_d.m_Center.x, m_d.m_Center.y, m_d.m_BaseRadius, rgv[0].x, rgv[0].y, rgv[3].x, rgv[3].y);
      psur->Arc(vendcenter.x, vendcenter.y, m_d.m_EndRadius, rgv[2].x, rgv[2].y, rgv[1].x, rgv[1].y);

      SetVertices(m_d.m_Center.x, m_d.m_Center.y, anglerad2, &vendcenter, rgv, m_d.m_BaseRadius, m_d.m_EndRadius);

      psur->SetLineColor(RGB(128, 128, 128), true, 0);

      psur->Line(rgv[0].x, rgv[0].y, rgv[1].x, rgv[1].y);
      psur->Line(rgv[2].x, rgv[2].y, rgv[3].x, rgv[3].y);

      psur->Arc(m_d.m_Center.x, m_d.m_Center.y, m_d.m_BaseRadius, rgv[0].x, rgv[0].y, rgv[3].x, rgv[3].y);
      psur->Arc(vendcenter.x, vendcenter.y, m_d.m_EndRadius, rgv[2].x, rgv[2].y, rgv[1].x, rgv[1].y);

      rgv[0].x = m_d.m_Center.x + sinf(anglerad) * (m_d.m_FlipperRadius + m_d.m_EndRadius);
      rgv[0].y = m_d.m_Center.y - cosf(anglerad) * (m_d.m_FlipperRadius + m_d.m_EndRadius);

      rgv[1].x = m_d.m_Center.x + sinf(anglerad2) * (m_d.m_FlipperRadius + m_d.m_EndRadius);
      rgv[1].y = m_d.m_Center.y - cosf(anglerad2) * (m_d.m_FlipperRadius + m_d.m_EndRadius);

      if (m_d.m_EndAngle < m_d.m_StartAngle)
         psur->Arc(m_d.m_Center.x, m_d.m_Center.y, m_d.m_FlipperRadius + m_d.m_EndRadius
         , rgv[0].x, rgv[0].y, rgv[1].x, rgv[1].y);
      else psur->Arc(m_d.m_Center.x, m_d.m_Center.y, m_d.m_FlipperRadius + m_d.m_EndRadius
         , rgv[1].x, rgv[1].y, rgv[0].x, rgv[0].y);

      m_d.m_FlipperRadius = m_d.m_FlipperRadiusMax - (m_d.m_FlipperRadiusMax - m_d.m_FlipperRadiusMin) * m_ptable->m_globalDifficulty;
      m_d.m_FlipperRadius = max(m_d.m_FlipperRadius, m_d.m_BaseRadius - m_d.m_EndRadius + 0.05f);

      SetVertices(m_d.m_Center.x, m_d.m_Center.y, anglerad, &vendcenter, rgv, m_d.m_BaseRadius, m_d.m_EndRadius);

      psur->SetObject(this);

      psur->Line(rgv[0].x, rgv[0].y, rgv[1].x, rgv[1].y);
      psur->Line(rgv[2].x, rgv[2].y, rgv[3].x, rgv[3].y);

      psur->Arc(m_d.m_Center.x, m_d.m_Center.y, m_d.m_BaseRadius, rgv[0].x, rgv[0].y, rgv[3].x, rgv[3].y);
      psur->Arc(vendcenter.x, vendcenter.y, m_d.m_EndRadius, rgv[2].x, rgv[2].y, rgv[1].x, rgv[1].y);

      SetVertices(m_d.m_Center.x, m_d.m_Center.y, anglerad2, &vendcenter, rgv, m_d.m_BaseRadius, m_d.m_EndRadius);

      psur->SetLineColor(RGB(128, 128, 128), true, 0);

      psur->Line(rgv[0].x, rgv[0].y, rgv[1].x, rgv[1].y);
      psur->Line(rgv[2].x, rgv[2].y, rgv[3].x, rgv[3].y);

      psur->Arc(m_d.m_Center.x, m_d.m_Center.y, m_d.m_BaseRadius, rgv[0].x, rgv[0].y, rgv[3].x, rgv[3].y);
      psur->Arc(vendcenter.x, vendcenter.y, m_d.m_EndRadius, rgv[2].x, rgv[2].y, rgv[1].x, rgv[1].y);

      rgv[0].x = m_d.m_Center.x + sinf(anglerad) * (m_d.m_FlipperRadius + m_d.m_EndRadius);
      rgv[0].y = m_d.m_Center.y - cosf(anglerad) * (m_d.m_FlipperRadius + m_d.m_EndRadius);

      rgv[1].x = m_d.m_Center.x + sinf(anglerad2) * (m_d.m_FlipperRadius + m_d.m_EndRadius);
      rgv[1].y = m_d.m_Center.y - cosf(anglerad2) * (m_d.m_FlipperRadius + m_d.m_EndRadius);

      if (m_d.m_EndAngle < m_d.m_StartAngle)
         psur->Arc(m_d.m_Center.x, m_d.m_Center.y, m_d.m_FlipperRadius + m_d.m_EndRadius
         , rgv[0].x, rgv[0].y, rgv[1].x, rgv[1].y);
      else psur->Arc(m_d.m_Center.x, m_d.m_Center.y, m_d.m_FlipperRadius + m_d.m_EndRadius
         , rgv[1].x, rgv[1].y, rgv[0].x, rgv[0].y);

   }

   m_d.m_FlipperRadius = m_d.m_FlipperRadiusMax;
}

void Flipper::SetObjectPos()
{
   g_pvp->SetObjectPosCur(m_d.m_Center.x, m_d.m_Center.y);
}

void Flipper::MoveOffset(const float dx, const float dy)
{
   m_d.m_Center.x += dx;
   m_d.m_Center.y += dy;

   m_ptable->SetDirtyDraw();
}

Vertex2D Flipper::GetCenter() const
{
   return m_d.m_Center;
}

void Flipper::PutCenter(const Vertex2D& pv)
{
   m_d.m_Center = pv;

   m_ptable->SetDirtyDraw();
}

void Flipper::SetDefaultPhysics(bool fromMouseClick)
{
   static const char regKey[] = "DefaultProps\\Flipper";

   HRESULT hr;
   float fTmp;

   hr = GetRegStringAsFloat(regKey, "Scatter", &fTmp);
   m_d.m_scatter = (hr == S_OK) && fromMouseClick ? fTmp : 0.0f;

   hr = GetRegStringAsFloat(regKey, "Strength", &fTmp);
   m_d.m_strength = (hr == S_OK) && fromMouseClick ? fTmp : 2200.0f;

   hr = GetRegStringAsFloat(regKey, "EOSTorque", &fTmp);
   m_d.m_torqueDamping = (hr == S_OK) && fromMouseClick ? fTmp : 0.75f;

   hr = GetRegStringAsFloat(regKey, "EOSTorqueAngle", &fTmp);
   m_d.m_torqueDampingAngle = (hr == S_OK) && fromMouseClick ? fTmp : 6.f;

   //m_d.m_angleEOS = 0;

   hr = GetRegStringAsFloat(regKey, "ReturnStrength", &fTmp);
   m_d.m_return = (hr == S_OK) && fromMouseClick ? fTmp : 0.058f;

   hr = GetRegStringAsFloat(regKey, "Mass", &fTmp);
   if (hr != S_OK)
      hr = GetRegStringAsFloat(regKey, "Speed", &fTmp); // previously Mass was called Speed, deprecated!
   m_d.m_mass = (hr == S_OK) && fromMouseClick ? fTmp : 1.0f;

   hr = GetRegStringAsFloat(regKey, "Elasticity", &fTmp);
   m_d.m_elasticity = (hr == S_OK) && fromMouseClick ? fTmp : 0.8f;

   hr = GetRegStringAsFloat(regKey, "ElasticityFalloff", &fTmp);
   m_d.m_elasticityFalloff = (hr == S_OK) && fromMouseClick ? fTmp : 0.43f;

   hr = GetRegStringAsFloat(regKey, "Friction", &fTmp);
   m_d.m_friction = (hr == S_OK) && fromMouseClick ? fTmp : 0.6f;

   hr = GetRegStringAsFloat(regKey, "RampUp", &fTmp);
   m_d.m_rampUp = (hr == S_OK) && fromMouseClick ? fTmp : 3.0f;
   
   m_d.m_OverridePhysics = 0;
}

STDMETHODIMP Flipper::InterfaceSupportsErrorInfo(REFIID riid)
{
   static const IID* arr[] =
   {
      &IID_IFlipper,
   };

   for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
   {
      if (InlineIsEqualGUID(*arr[i], riid))
         return S_OK;
   }
   return S_FALSE;
}

STDMETHODIMP Flipper::RotateToEnd() // power stroke to hit ball, key/button down/pressed
{
   if (m_phitflipper)
   {
      g_pplayer->m_pininput.m_leftkey_down_usec_rotate_to_end = usec(); // debug only
      g_pplayer->m_pininput.m_leftkey_down_frame_rotate_to_end = g_pplayer->m_overall_frames;

      m_phitflipper->m_flipperMover.m_EnableRotateEvent = 1;
      m_phitflipper->m_flipperMover.SetSolenoidState(true);
   }

   return S_OK;
}

STDMETHODIMP Flipper::RotateToStart() // return to park, key/button up/released
{
   if (m_phitflipper)
   {
      m_phitflipper->m_flipperMover.m_EnableRotateEvent = -1;
      m_phitflipper->m_flipperMover.SetSolenoidState(false);
   }

   return S_OK;
}
void Flipper::RenderDynamic(RenderDevice* pd3dDevice)
{
   TRACE_FUNCTION();

   if (m_phitflipper && !m_phitflipper->m_flipperMover.m_fVisible)
      return;
   if (m_phitflipper == NULL && !m_d.m_fVisible)
      return;

   if (m_ptable->m_fReflectionEnabled && !m_d.m_fReflectionEnabled)
      return;

   const Material * mat = m_ptable->GetMaterial(m_d.m_szMaterial);
   pd3dDevice->basicShader->SetMaterial(mat);

   Texture * const pin = m_ptable->GetImage(m_d.m_szImage);
   if (pin)
   {
      pd3dDevice->basicShader->SetTechnique(mat->m_bIsMetal ? "basic_with_texture_isMetal" : "basic_with_texture_isNotMetal");
      pd3dDevice->basicShader->SetTexture("Texture0", pin, false);
      pd3dDevice->basicShader->SetAlphaTestValue(pin->m_alphaTestValue * (float)(1.0 / 255.0));

      //g_pplayer->m_pin3d.SetTextureFilter(0, TEXTURE_MODE_TRILINEAR);
      // accomodate models with UV coords outside of [0,1]
      //pd3dDevice->SetTextureAddressMode(0, RenderDevice::TEX_WRAP);
   }
   else
      pd3dDevice->basicShader->SetTechnique(mat->m_bIsMetal ? "basic_without_texture_isMetal" : "basic_without_texture_isNotMetal");

   pd3dDevice->SetRenderState(RenderDevice::DEPTHBIAS, 0);
   pd3dDevice->SetRenderState(RenderDevice::ZWRITEENABLE, TRUE);
   pd3dDevice->SetRenderState(RenderDevice::CULLMODE, D3DCULL_CCW);

   Matrix3D matTrafo;
   matTrafo.SetIdentity();
   matTrafo._41 = m_d.m_Center.x;
   matTrafo._42 = m_d.m_Center.y;
   if (m_phitflipper)
   {
      Matrix3D matTemp;
      matTemp.RotateZMatrix(m_phitflipper->m_flipperMover.m_angleCur);
      matTrafo.Multiply(matTemp, matTrafo);
   }
   g_pplayer->UpdateBasicShaderMatrix(matTrafo);
   pd3dDevice->basicShader->Begin(0);
   pd3dDevice->DrawIndexedPrimitiveVB(D3DPT_TRIANGLELIST, MY_D3DFVF_NOTEX2_VERTEX, vertexBuffer, 0, flipperBaseVertices, indexBuffer, 0, flipperBaseNumIndices);
   pd3dDevice->basicShader->End();

   //render rubber
   if (m_d.m_rubberthickness > 0.f)
   {
      mat = m_ptable->GetMaterial(m_d.m_szRubberMaterial);
      if (pin)
        pd3dDevice->basicShader->SetTechnique(mat->m_bIsMetal ? "basic_with_texture_isMetal" : "basic_with_texture_isNotMetal");
      else
        pd3dDevice->basicShader->SetTechnique(mat->m_bIsMetal ? "basic_without_texture_isMetal" : "basic_without_texture_isNotMetal");
      pd3dDevice->basicShader->SetMaterial(mat);

      pd3dDevice->basicShader->Begin(0);
      pd3dDevice->DrawIndexedPrimitiveVB(D3DPT_TRIANGLELIST, MY_D3DFVF_NOTEX2_VERTEX, vertexBuffer, flipperBaseVertices, flipperBaseVertices, indexBuffer, 0, flipperBaseNumIndices);
      pd3dDevice->basicShader->End();
   }
   g_pplayer->UpdateBasicShaderMatrix();
}

void Flipper::ExportMesh(FILE *f)
{
   char name[MAX_PATH];
   char subObjName[MAX_PATH];
   WideCharToMultiByte(CP_ACP, 0, m_wzName, -1, name, MAX_PATH, NULL, NULL);
   Matrix3D matTrafo, matTemp;
   matTrafo.SetIdentity();
   matTemp.SetIdentity();
   matTrafo._41 = m_d.m_Center.x;
   matTrafo._42 = m_d.m_Center.y;
   matTemp.RotateZMatrix(ANGTORAD(m_d.m_StartAngle));
   matTrafo.Multiply(matTemp, matTrafo);

   Vertex3D_NoTex2 *flipper = new Vertex3D_NoTex2[flipperBaseVertices * 2];
   GenerateBaseMesh(flipper);

   {
   Vertex3D_NoTex2 *buf = flipper;
   for (int i = 0; i < flipperBaseVertices; i++)
   {
      Vertex3Ds vert(buf[i].x, buf[i].y, buf[i].z);
      vert = matTrafo.MultiplyVector(vert);
      buf[i].x = vert.x;
      buf[i].y = vert.y;
      buf[i].z = vert.z;

      vert = Vertex3Ds(buf[i].nx, buf[i].ny, buf[i].nz);
      vert = matTrafo.MultiplyVectorNoTranslate(vert);
      buf[i].nx = vert.x;
      buf[i].ny = vert.y;
      buf[i].nz = vert.z;
   }
   }

   strcpy_s(subObjName, name);
   strcat_s(subObjName, "Base");
   WaveFrontObj_WriteObjectName(f, subObjName);
   WaveFrontObj_WriteVertexInfo(f, flipper, flipperBaseVertices);
   const Material * mat = m_ptable->GetMaterial(m_d.m_szMaterial);
   WaveFrontObj_WriteMaterial(m_d.m_szMaterial, NULL, mat);
   WaveFrontObj_UseTexture(f, m_d.m_szMaterial);
   WaveFrontObj_WriteFaceInfoList(f, flipperBaseIndices, flipperBaseNumIndices);
   WaveFrontObj_UpdateFaceOffset(flipperBaseVertices);
   if (m_d.m_rubberthickness > 0.f)
   {
      Vertex3D_NoTex2 *buf = &flipper[flipperBaseVertices];
      for (int i = 0; i < flipperBaseVertices; i++)
      {
         Vertex3Ds vert(buf[i].x, buf[i].y, buf[i].z);
         vert = matTrafo.MultiplyVector(vert);
         buf[i].x = vert.x;
         buf[i].y = vert.y;
         buf[i].z = vert.z;

         vert = Vertex3Ds(buf[i].nx, buf[i].ny, buf[i].nz);
         vert = matTrafo.MultiplyVectorNoTranslate(vert);
         buf[i].nx = vert.x;
         buf[i].ny = vert.y;
         buf[i].nz = vert.z;
      }

      strcpy_s(subObjName, name);
      strcat_s(subObjName, "Rubber");
      WaveFrontObj_WriteObjectName(f, subObjName);
      WaveFrontObj_WriteVertexInfo(f, &flipper[flipperBaseVertices], flipperBaseVertices);
      mat = m_ptable->GetMaterial(m_d.m_szRubberMaterial);
      WaveFrontObj_WriteMaterial(m_d.m_szRubberMaterial, NULL, mat);
      WaveFrontObj_UseTexture(f, m_d.m_szRubberMaterial);
      WaveFrontObj_WriteFaceInfoList(f, flipperBaseIndices, flipperBaseNumIndices);
      WaveFrontObj_UpdateFaceOffset(flipperBaseVertices);
   }

   delete [] flipper;
}

void Flipper::GenerateBaseMesh(Vertex3D_NoTex2 *buf)
{
   Matrix3D fullMatrix;

   fullMatrix.RotateZMatrix(ANGTORAD(180.0f));

   const float height = m_ptable->GetSurfaceHeight(m_d.m_szSurface, m_d.m_Center.x, m_d.m_Center.y);
   const float baseScale = 10.0f;
   const float tipScale = 10.0f;
   const float baseRadius = m_d.m_BaseRadius - m_d.m_rubberthickness;
   const float endRadius = m_d.m_EndRadius - m_d.m_rubberthickness;
   Vertex3D_NoTex2 *temp = new Vertex3D_NoTex2[flipperBaseVertices];

   // scale the base and tip
   memcpy(temp, flipperBaseMesh, sizeof(Vertex3D_NoTex2)*flipperBaseVertices);
   for (int t = 0; t < 13; t++)
   {
      for (int i = 0; i < flipperBaseVertices; i++)
      {
         if (temp[i].x == vertsBaseBottom[t].x && temp[i].y == vertsBaseBottom[t].y && temp[i].z == vertsBaseBottom[t].z)
         {
            temp[i].x *= baseRadius*baseScale;
            temp[i].y *= baseRadius*baseScale;
         }
         if (temp[i].x == vertsTipBottom[t].x && temp[i].y == vertsTipBottom[t].y && temp[i].z == vertsTipBottom[t].z)
         {
            temp[i].x *= endRadius * tipScale;
            temp[i].y *= endRadius * tipScale;
            temp[i].y += m_d.m_FlipperRadius - endRadius*7.9f;
         }
         if (temp[i].x == vertsBaseTop[t].x && temp[i].y == vertsBaseTop[t].y && temp[i].z == vertsBaseTop[t].z)
         {
            temp[i].x *= baseRadius*baseScale;
            temp[i].y *= baseRadius*baseScale;
         }
         if (temp[i].x == vertsTipTop[t].x && temp[i].y == vertsTipTop[t].y && temp[i].z == vertsTipTop[t].z)
         {
            temp[i].x *= endRadius*tipScale;
            temp[i].y *= endRadius * tipScale;
            temp[i].y += m_d.m_FlipperRadius - endRadius * 7.9f;
         }
      }
   }
   for (int i = 0; i < flipperBaseVertices; i++)
   {
      Vertex3Ds vert = fullMatrix.MultiplyVector(Vertex3Ds(temp[i].x, temp[i].y, temp[i].z));
      buf[i].x = vert.x;
      buf[i].y = vert.y;
      buf[i].z = vert.z*m_d.m_height*m_ptable->m_BG_scalez[m_ptable->m_BG_current_set] + height;

      vert = fullMatrix.MultiplyVectorNoTranslate(Vertex3Ds(flipperBaseMesh[i].nx, flipperBaseMesh[i].ny, flipperBaseMesh[i].nz));
      buf[i].nx = vert.x;
      buf[i].ny = vert.y;
      buf[i].nz = vert.z;
      buf[i].tu = flipperBaseMesh[i].tu;
      buf[i].tv = flipperBaseMesh[i].tv;
   }

   //rubber
   if (m_d.m_rubberthickness > 0.f)
   {
      const float rubberBaseScale = 10.0f;
      const float rubberTipScale = 10.0f;
      memcpy(temp, flipperBaseMesh, sizeof(Vertex3D_NoTex2)*flipperBaseVertices);
      for (int t = 0; t < 13; t++)
      {
         for (int i = 0; i < flipperBaseVertices; i++)
         {
            if (temp[i].x == vertsBaseBottom[t].x && temp[i].y == vertsBaseBottom[t].y && temp[i].z == vertsBaseBottom[t].z)
            {
               temp[i].x *= m_d.m_BaseRadius*rubberBaseScale;
               temp[i].y *= m_d.m_BaseRadius*rubberBaseScale;
            }
            if (temp[i].x == vertsTipBottom[t].x && temp[i].y == vertsTipBottom[t].y && temp[i].z == vertsTipBottom[t].z)
            {
               temp[i].x *= m_d.m_EndRadius * rubberTipScale;
               temp[i].y *= m_d.m_EndRadius * rubberTipScale;
               temp[i].y += m_d.m_FlipperRadius - m_d.m_EndRadius*7.9f;
            }
            if (temp[i].x == vertsBaseTop[t].x && temp[i].y == vertsBaseTop[t].y && temp[i].z == vertsBaseTop[t].z)
            {
               temp[i].x *= m_d.m_BaseRadius*rubberBaseScale;
               temp[i].y *= m_d.m_BaseRadius*rubberBaseScale;
            }
            if (temp[i].x == vertsTipTop[t].x && temp[i].y == vertsTipTop[t].y && temp[i].z == vertsTipTop[t].z)
            {
               temp[i].x *= m_d.m_EndRadius*rubberTipScale;
               temp[i].y *= m_d.m_EndRadius * rubberTipScale;
               temp[i].y += m_d.m_FlipperRadius - m_d.m_EndRadius*7.9f;
            }
         }
      }

      for (int i = 0; i < flipperBaseVertices; i++)
      {
         Vertex3Ds vert = fullMatrix.MultiplyVector(Vertex3Ds(temp[i].x, temp[i].y, temp[i].z));
         buf[i + flipperBaseVertices].x = vert.x;
         buf[i + flipperBaseVertices].y = vert.y;
         buf[i + flipperBaseVertices].z = vert.z*m_d.m_rubberwidth*m_ptable->m_BG_scalez[m_ptable->m_BG_current_set] + (height + m_d.m_rubberheight);

         vert = fullMatrix.MultiplyVectorNoTranslate(Vertex3Ds(flipperBaseMesh[i].nx, flipperBaseMesh[i].ny, flipperBaseMesh[i].nz));
         buf[i + flipperBaseVertices].nx = vert.x;
         buf[i + flipperBaseVertices].ny = vert.y;
         buf[i + flipperBaseVertices].nz = vert.z;
         buf[i + flipperBaseVertices].tu = flipperBaseMesh[i].tu;
         buf[i + flipperBaseVertices].tv = flipperBaseMesh[i].tv + 0.5f;
      }
   }

   delete [] temp;
}

void Flipper::RenderSetup(RenderDevice* pd3dDevice)
{
   if (indexBuffer)
      indexBuffer->release();
   indexBuffer = pd3dDevice->CreateAndFillIndexBuffer(flipperBaseNumIndices, flipperBaseIndices);

   if (vertexBuffer)
      vertexBuffer->release();
   pd3dDevice->CreateVertexBuffer(flipperBaseVertices * 2, 0, MY_D3DFVF_NOTEX2_VERTEX, &vertexBuffer);

   Vertex3D_NoTex2 *buf;
   vertexBuffer->lock(0, 0, (void**)&buf, VertexBuffer::WRITEONLY);
   GenerateBaseMesh(buf);
   vertexBuffer->unlock();
}

void Flipper::RenderStatic(RenderDevice* pd3dDevice)
{
}

HRESULT Flipper::SaveData(IStream *pstm, HCRYPTHASH hcrypthash)
{
   BiffWriter bw(pstm, hcrypthash);

   bw.WriteStruct(FID(VCEN), &m_d.m_Center, sizeof(Vertex2D));
   bw.WriteFloat(FID(BASR), m_d.m_BaseRadius);
   bw.WriteFloat(FID(ENDR), m_d.m_EndRadius);
   bw.WriteFloat(FID(FLPR), m_d.m_FlipperRadiusMax);
   //bw.WriteFloat(FID(FAEOS), m_d.m_angleEOS);
   bw.WriteFloat(FID(FRTN), m_d.m_return);
   bw.WriteFloat(FID(ANGS), m_d.m_StartAngle);
   bw.WriteFloat(FID(ANGE), m_d.m_EndAngle);
   bw.WriteInt(FID(OVRP), m_d.m_OverridePhysics);
   bw.WriteFloat(FID(FORC), m_d.m_mass);
   bw.WriteBool(FID(TMON), m_d.m_tdr.m_fTimerEnabled);
   bw.WriteInt(FID(TMIN), m_d.m_tdr.m_TimerInterval);
   bw.WriteString(FID(SURF), m_d.m_szSurface);
   bw.WriteString(FID(MATR), m_d.m_szMaterial);
   bw.WriteWideString(FID(NAME), (WCHAR *)m_wzName);
   bw.WriteString(FID(RUMA), m_d.m_szRubberMaterial);
   bw.WriteInt(FID(RTHK), (int)m_d.m_rubberthickness); //!! deprecated, remove
   bw.WriteFloat(FID(RTHF), m_d.m_rubberthickness);
   bw.WriteInt(FID(RHGT), (int)m_d.m_rubberheight); //!! deprecated, remove
   bw.WriteFloat(FID(RHGF), m_d.m_rubberheight);
   bw.WriteInt(FID(RWDT), (int)m_d.m_rubberwidth); //!! deprecated, remove
   bw.WriteFloat(FID(RWDF), m_d.m_rubberwidth);
   bw.WriteFloat(FID(STRG), m_d.m_strength);
   bw.WriteFloat(FID(ELAS), m_d.m_elasticity);
   bw.WriteFloat(FID(ELFO), m_d.m_elasticityFalloff);
   bw.WriteFloat(FID(FRIC), m_d.m_friction);
   bw.WriteFloat(FID(RPUP), m_d.m_rampUp);
   bw.WriteFloat(FID(SCTR), m_d.m_scatter);
   bw.WriteFloat(FID(TODA), m_d.m_torqueDamping);
   bw.WriteFloat(FID(TDAA), m_d.m_torqueDampingAngle);
   bw.WriteBool(FID(VSBL), m_d.m_fVisible);
   bw.WriteBool(FID(ENBL), m_d.m_fEnabled);
   bw.WriteFloat(FID(FRMN), m_d.m_FlipperRadiusMin);
   bw.WriteFloat(FID(FHGT), m_d.m_height);
   bw.WriteString(FID(IMAG), m_d.m_szImage);
   bw.WriteBool(FID(REEN), m_d.m_fReflectionEnabled);


   ISelect::SaveData(pstm, hcrypthash);

   bw.WriteTag(FID(ENDB));

   return S_OK;
}

HRESULT Flipper::InitLoad(IStream *pstm, PinTable *ptable, int *pid, int version, HCRYPTHASH hcrypthash, HCRYPTKEY hcryptkey)
{
   SetDefaults(false);

   BiffReader br(pstm, this, pid, version, hcrypthash, hcryptkey);

   m_ptable = ptable;

   br.Load();
   return S_OK;
}

BOOL Flipper::LoadToken(int id, BiffReader *pbr)
{
   if (id == FID(PIID))
   {
      pbr->GetInt((int *)pbr->m_pdata);
   }
   else if (id == FID(VCEN))
   {
      pbr->GetStruct(&m_d.m_Center, sizeof(Vertex2D));
   }
   else if (id == FID(BASR))
   {
      pbr->GetFloat(&m_d.m_BaseRadius);
   }
   else if (id == FID(ENDR))
   {
      pbr->GetFloat(&m_d.m_EndRadius);
   }
   else if (id == FID(FLPR))
   {
      pbr->GetFloat(&m_d.m_FlipperRadiusMax);
   }
   /*else if (id == FID(FAEOS))
   {
   pbr->GetFloat(&m_d.m_angleEOS);
   }*/
   else if (id == FID(FRTN))
   {
      pbr->GetFloat(&m_d.m_return);
   }
   else if (id == FID(ANGS))
   {
      pbr->GetFloat(&m_d.m_StartAngle);
   }
   else if (id == FID(ANGE))
   {
      pbr->GetFloat(&m_d.m_EndAngle);
   }
   else if (id == FID(OVRP))
   {
      pbr->GetInt(&m_d.m_OverridePhysics);
   }
   else if (id == FID(FORC))
   {
      pbr->GetFloat(&m_d.m_mass);
   }
   else if (id == FID(TMON))
   {
      pbr->GetBool(&m_d.m_tdr.m_fTimerEnabled);
   }
   else if (id == FID(TMIN))
   {
      pbr->GetInt(&m_d.m_tdr.m_TimerInterval);
      //m_d.m_tdr.m_TimerInterval = INT(m_d.m_tdr.m_TimerInterval);
      if (m_d.m_tdr.m_TimerInterval < 1)
         m_d.m_tdr.m_TimerInterval = 100;
   }
   else if (id == FID(SURF))
   {
      pbr->GetString(m_d.m_szSurface);
   }
   else if (id == FID(MATR))
   {
      pbr->GetString(m_d.m_szMaterial);
   }
   else if (id == FID(RUMA))
   {
      pbr->GetString(m_d.m_szRubberMaterial);
   }
   else if (id == FID(NAME))
   {
      pbr->GetWideString((WCHAR *)m_wzName);
   }
   else if (id == FID(RTHK)) //!! deprecated, remove
   {
      int rt;
      pbr->GetInt(&rt);
      m_d.m_rubberthickness = (float)rt;
   }
   else if (id == FID(RTHF))
   {
      pbr->GetFloat(&m_d.m_rubberthickness);
   }
   else if (id == FID(RHGT)) //!! deprecated, remove
   {
      int rh;
      pbr->GetInt(&rh);
      m_d.m_rubberheight = (float)rh;
   }
   else if (id == FID(RHGF))
   {
      pbr->GetFloat(&m_d.m_rubberheight);
   }
   else if (id == FID(RWDT)) //!! deprecated, remove
   {
      int rw;
      pbr->GetInt(&rw);
      m_d.m_rubberwidth = (float)rw;
   }
   else if (id == FID(RWDF))
   {
      pbr->GetFloat(&m_d.m_rubberwidth);
   }
   else if (id == FID(FHGT))
   {
      pbr->GetFloat(&m_d.m_height);
   }
   else if (id == FID(STRG))
   {
      pbr->GetFloat(&m_d.m_strength);
   }
   else if (id == FID(ELAS))
   {
      pbr->GetFloat(&m_d.m_elasticity);
   }
   else if (id == FID(ELFO))
   {
      pbr->GetFloat(&m_d.m_elasticityFalloff);
   }
   else if (id == FID(FRIC))
   {
      pbr->GetFloat(&m_d.m_friction);
   }
   else if (id == FID(RPUP))
   {
      pbr->GetFloat(&m_d.m_rampUp);
   }
   else if (id == FID(SCTR))
   {
      pbr->GetFloat(&m_d.m_scatter);
   }
   else if ( id == FID( TODA ) )
   {
      pbr->GetFloat( &m_d.m_torqueDamping );
   }
   else if (id == FID(TDAA))
   {
      pbr->GetFloat(&m_d.m_torqueDampingAngle);
   }
   else if (id == FID(FRMN))
   {
      pbr->GetFloat(&m_d.m_FlipperRadiusMin);
   }
   else if (id == FID(VSBL))
   {
      pbr->GetBool(&m_d.m_fVisible);
   }
   else if (id == FID(ENBL))
   {
      pbr->GetBool(&m_d.m_fEnabled);
   }
   else if (id == FID(REEN))
   {
      pbr->GetBool(&m_d.m_fReflectionEnabled);
   }
   else if (id == FID(IMAG))
   {
      pbr->GetString(m_d.m_szImage);
   }
   else
   {
      ISelect::LoadToken(id, pbr);
   }

   return fTrue;
}

HRESULT Flipper::InitPostLoad()
{
   if (m_d.m_height > 1000.0f)
      m_d.m_height = 50.0f;
   if (m_d.m_rubberheight > 1000.f)
      m_d.m_rubberheight = 8.f;
   if (m_d.m_rubberthickness > 0.f && m_d.m_height > 16.0f && m_d.m_rubberwidth == 0.f)
      m_d.m_rubberwidth = m_d.m_height - 16.0f;
   if (m_d.m_rubberwidth > 1000.f)
      m_d.m_rubberwidth = m_d.m_height - 16.0f;

   return S_OK;
}

STDMETHODIMP Flipper::get_BaseRadius(float *pVal)
{
   *pVal = m_d.m_BaseRadius;

   return S_OK;
}

STDMETHODIMP Flipper::put_BaseRadius(float newVal)
{
   STARTUNDO

   m_d.m_BaseRadius = newVal;

   STOPUNDO;

   return S_OK;
}

STDMETHODIMP Flipper::get_EndRadius(float *pVal)
{
   *pVal = m_d.m_EndRadius;

   return S_OK;
}

STDMETHODIMP Flipper::put_EndRadius(float newVal)
{
   STARTUNDO

   m_d.m_EndRadius = newVal;

   STOPUNDO

   return S_OK;
}

STDMETHODIMP Flipper::get_Length(float *pVal)
{
   *pVal = m_d.m_FlipperRadiusMax;
   UpdateUnitsInfo();

   return S_OK;
}

STDMETHODIMP Flipper::put_Length(float newVal)
{
   STARTUNDO

   m_d.m_FlipperRadiusMax = newVal;
   UpdateUnitsInfo();

   STOPUNDO

   return S_OK;
}

STDMETHODIMP Flipper::get_EOSTorque(float *pVal)
{
   *pVal = (m_d.m_OverridePhysics || (m_ptable->m_overridePhysicsFlipper && m_ptable->m_overridePhysics)) ? m_d.m_OverrideTorqueDamping : m_d.m_torqueDamping;

   return S_OK;
}

STDMETHODIMP Flipper::put_EOSTorque(float newVal)
{
    if ( m_phitflipper )
    {
        if (!(m_d.m_OverridePhysics || (m_ptable->m_overridePhysicsFlipper && m_ptable->m_overridePhysics)))
           m_d.m_torqueDamping = newVal;
    }
    else
    {
        STARTUNDO
        m_d.m_torqueDamping = newVal;
        STOPUNDO
    }

    return S_OK;
}

STDMETHODIMP Flipper::get_EOSTorqueAngle(float *pVal)
{
   *pVal = (m_d.m_OverridePhysics || (m_ptable->m_overridePhysicsFlipper && m_ptable->m_overridePhysics)) ? m_d.m_OverrideTorqueDampingAngle : m_d.m_torqueDampingAngle;

   return S_OK;
}

STDMETHODIMP Flipper::put_EOSTorqueAngle(float newVal)
{
    if (m_phitflipper)
    {
        if (!(m_d.m_OverridePhysics || (m_ptable->m_overridePhysicsFlipper && m_ptable->m_overridePhysics)))
           m_d.m_torqueDampingAngle = newVal;    
    }
    else
    {
        STARTUNDO
        m_d.m_torqueDampingAngle = newVal;
        STOPUNDO
    }

    return S_OK;
}

STDMETHODIMP Flipper::get_StartAngle(float *pVal)
{
   if (m_phitflipper)
   {
      *pVal = RADTOANG(m_phitflipper->m_flipperMover.m_angleStart);
   }
   else
      *pVal = m_d.m_StartAngle;

   return S_OK;
}

STDMETHODIMP Flipper::put_StartAngle(float newVal)
{
   if (m_phitflipper)
   {
      m_phitflipper->m_flipperMover.SetStartAngle(ANGTORAD(newVal));
   }
   else
   {
      STARTUNDO
      m_d.m_StartAngle = newVal;
      STOPUNDO
   }

   return S_OK;
}

STDMETHODIMP Flipper::get_EndAngle(float *pVal)
{
   if (m_phitflipper)
   {
      *pVal = RADTOANG(m_phitflipper->m_flipperMover.m_angleEnd);
   }
   else
      *pVal = m_d.m_EndAngle;

   return S_OK;
}

STDMETHODIMP Flipper::put_EndAngle(float newVal)
{
   if (m_phitflipper)
   {
      m_phitflipper->m_flipperMover.SetEndAngle(ANGTORAD(newVal));
   }
   else
   {
      STARTUNDO
      m_d.m_EndAngle = newVal;
      STOPUNDO;
   }

   return S_OK;
}

STDMETHODIMP Flipper::get_CurrentAngle(float *pVal)
{
   if (m_phitflipper)
   {
      *pVal = RADTOANG(m_phitflipper->m_flipperMover.m_angleCur);
      return S_OK;
   }
   else
      return E_FAIL;
}

STDMETHODIMP Flipper::get_X(float *pVal)
{
   *pVal = m_d.m_Center.x;

   return S_OK;
}

STDMETHODIMP Flipper::put_X(float newVal)
{
   STARTUNDO

   m_d.m_Center.x = newVal;

   STOPUNDO

   return S_OK;
}

STDMETHODIMP Flipper::get_Y(float *pVal)
{
   *pVal = m_d.m_Center.y;

   return S_OK;
}

STDMETHODIMP Flipper::put_Y(float newVal)
{
   STARTUNDO

   m_d.m_Center.y = newVal;

   STOPUNDO

   return S_OK;
}

STDMETHODIMP Flipper::get_Surface(BSTR *pVal)
{
   WCHAR wz[512];

   MultiByteToWideChar(CP_ACP, 0, m_d.m_szSurface, -1, wz, 32);
   *pVal = SysAllocString(wz);

   return S_OK;
}

STDMETHODIMP Flipper::put_Surface(BSTR newVal)
{
   STARTUNDO

   WideCharToMultiByte(CP_ACP, 0, newVal, -1, m_d.m_szSurface, 32, NULL, NULL);

   STOPUNDO

   return S_OK;
}

STDMETHODIMP Flipper::get_Material(BSTR *pVal)
{
   WCHAR wz[512];

   MultiByteToWideChar(CP_ACP, 0, m_d.m_szMaterial, -1, wz, 32);
   *pVal = SysAllocString(wz);

   return S_OK;
}

STDMETHODIMP Flipper::put_Material(BSTR newVal)
{
   STARTUNDO

   WideCharToMultiByte(CP_ACP, 0, newVal, -1, m_d.m_szMaterial, 32, NULL, NULL);

   STOPUNDO

   return S_OK;
}

/*STDMETHODIMP Flipper::get_AngleEOS(float *pVal)
{
   *pVal = m_d.m_angleEOS;

   return S_OK;
}

STDMETHODIMP Flipper::put_AngleEOS(float newVal)
{
   STARTUNDO

   m_d.m_angleEOS = newVal;

   STOPUNDO

   return S_OK;
}*/

void Flipper::GetDialogPanes(vector<PropertyPane*> &pvproppane)
{
   PropertyPane *pproppane;

   pproppane = new PropertyPane(IDD_PROP_NAME, NULL);
   pvproppane.push_back(pproppane);

   pproppane = new PropertyPane(IDD_PROPFLIPPER_VISUALS, IDS_VISUALS);
   pvproppane.push_back(pproppane);

   pproppane = new PropertyPane(IDD_PROPFLIPPER_POSITION, IDS_POSITION);
   pvproppane.push_back(pproppane);

   pproppane = new PropertyPane(IDD_PROPFLIPPER_PHYSICS, IDS_PHYSICS);
   pvproppane.push_back(pproppane);

   pproppane = new PropertyPane(IDD_PROP_TIMER, IDS_MISC);
   pvproppane.push_back(pproppane);
}

STDMETHODIMP Flipper::get_Mass(float *pVal)
{
    if (m_phitflipper)
    {
        *pVal = m_phitflipper->m_flipperMover.GetMass();
    }
    else
        *pVal = m_d.m_mass;

   return S_OK;
}

STDMETHODIMP Flipper::put_Mass(float newVal)
{
   if (m_phitflipper)
   {
      if (!(m_d.m_OverridePhysics || (m_ptable->m_overridePhysicsFlipper && m_ptable->m_overridePhysics)))
         m_phitflipper->m_flipperMover.SetMass(newVal);
   }
   else
   {
      STARTUNDO
      m_d.m_mass = newVal;
      STOPUNDO
   }

   return S_OK;
}

STDMETHODIMP Flipper::get_OverridePhysics(PhysicsSet *pVal)
{
   *pVal = (PhysicsSet)m_d.m_OverridePhysics;

   return S_OK;
}

STDMETHODIMP Flipper::put_OverridePhysics(PhysicsSet newVal)
{
   STARTUNDO
   m_d.m_OverridePhysics = (int)newVal;
   STOPUNDO

   if(m_phitflipper)
   {
      UpdatePhysicsSettings();
      m_phitflipper->UpdatePhysicsFromFlipper();
   }

   return S_OK;
}

STDMETHODIMP Flipper::get_RubberMaterial(BSTR *pVal)
{
   WCHAR wz[512];

   MultiByteToWideChar(CP_ACP, 0, m_d.m_szRubberMaterial, -1, wz, 32);
   *pVal = SysAllocString(wz);

   return S_OK;
}

STDMETHODIMP Flipper::put_RubberMaterial(BSTR newVal)
{
   STARTUNDO

   WideCharToMultiByte(CP_ACP, 0, newVal, -1, m_d.m_szRubberMaterial, 32, NULL, NULL);

   STOPUNDO

   return S_OK;
}

STDMETHODIMP Flipper::get_RubberThickness(float *pVal)
{
   *pVal = m_d.m_rubberthickness;

   return S_OK;
}

STDMETHODIMP Flipper::get_RubberHeight(float *pVal)
{
   *pVal = m_d.m_rubberheight;
   UpdateUnitsInfo();

   return S_OK;
}

STDMETHODIMP Flipper::get_RubberWidth(float *pVal)
{
   *pVal = m_d.m_rubberwidth;
   UpdateUnitsInfo();

   return S_OK;
}

STDMETHODIMP Flipper::put_RubberThickness(float newVal)
{
   STARTUNDO

   m_d.m_rubberthickness = newVal;

   STOPUNDO

   return S_OK;
}

STDMETHODIMP Flipper::put_RubberHeight(float newVal)
{
   if (newVal < 0.f) newVal = 0.f;
   else if (newVal > 1000.f) newVal = 50.f; //!! legacy, deprecated

   STARTUNDO

   m_d.m_rubberheight = newVal;
   UpdateUnitsInfo();

   STOPUNDO

   return S_OK;
}

STDMETHODIMP Flipper::put_RubberWidth(float newVal)
{
   STARTUNDO

   m_d.m_rubberwidth = newVal;
   UpdateUnitsInfo();

   STOPUNDO

   return S_OK;
}

STDMETHODIMP Flipper::get_Strength(float *pVal)
{
   if (m_phitflipper)
   {
       *pVal = m_phitflipper->m_flipperMover.GetStrength();
   }
   else
       *pVal = m_d.m_strength;

   return S_OK;
}

STDMETHODIMP Flipper::put_Strength(float newVal)
{
   if (m_phitflipper)
   {
      if(!(m_d.m_OverridePhysics || (m_ptable->m_overridePhysicsFlipper && m_ptable->m_overridePhysics)))
         m_d.m_strength = newVal;
   }
   else
   {
      STARTUNDO

      m_d.m_strength = newVal;

      STOPUNDO
   }

   return S_OK;
}

STDMETHODIMP Flipper::get_Visible(VARIANT_BOOL *pVal)
{
   *pVal = (VARIANT_BOOL)FTOVB(m_phitflipper ? m_phitflipper->m_flipperMover.m_fVisible : m_d.m_fVisible);

   return S_OK;
}

STDMETHODIMP Flipper::put_Visible(VARIANT_BOOL newVal)
{
   if (m_phitflipper)
   {
      m_phitflipper->m_flipperMover.m_fVisible = VBTOF(newVal); //m_d.m_fVisible
   }
   else
   {
      STARTUNDO
      m_d.m_fVisible = VBTOF(newVal);
      STOPUNDO
   }
   return S_OK;
}

STDMETHODIMP Flipper::get_Enabled(VARIANT_BOOL *pVal)
{
   *pVal = (VARIANT_BOOL)FTOVB(m_phitflipper ? m_phitflipper->m_flipperMover.m_fEnabled : m_d.m_fEnabled);

   return S_OK;
}

STDMETHODIMP Flipper::put_Enabled(VARIANT_BOOL newVal)
{
   if (m_phitflipper)
   {
      m_phitflipper->m_flipperMover.m_fEnabled = VBTOF(newVal); //m_d.m_fVisible
   }
   else
   {
      STARTUNDO
      m_d.m_fEnabled = VBTOF(newVal);
      STOPUNDO
   }
   return S_OK;
}

STDMETHODIMP Flipper::get_Elasticity(float *pVal)
{
   *pVal = m_phitflipper ? m_phitflipper->m_elasticity : m_d.m_elasticity;

   return S_OK;
}

STDMETHODIMP Flipper::put_Elasticity(float newVal)
{
   if (m_phitflipper)
   {
      if(!(m_d.m_OverridePhysics || (m_ptable->m_overridePhysicsFlipper && m_ptable->m_overridePhysics)))
         m_phitflipper->m_elasticity = newVal;
   }
   else
   {
      STARTUNDO
      m_d.m_elasticity = newVal;
      STOPUNDO
   }

   return S_OK;
}

STDMETHODIMP Flipper::get_Scatter(float *pVal)
{
   *pVal = m_phitflipper ? RADTOANG(m_phitflipper->m_scatter) : m_d.m_scatter;

   return S_OK;
}

STDMETHODIMP Flipper::put_Scatter(float newVal)
{
   if (m_phitflipper)
   {
      if(!(m_d.m_OverridePhysics || (m_ptable->m_overridePhysicsFlipper && m_ptable->m_overridePhysics)))
         m_phitflipper->m_scatter = ANGTORAD(newVal);
   }
   else
   {
      STARTUNDO
      m_d.m_scatter = newVal;
      STOPUNDO
   }

   return S_OK;
}

STDMETHODIMP Flipper::get_ElasticityFalloff(float *pVal)
{
   *pVal = m_phitflipper ? m_phitflipper->m_elasticityFalloff : m_d.m_elasticityFalloff;

   return S_OK;
}

STDMETHODIMP Flipper::put_ElasticityFalloff(float newVal)
{
   if (m_phitflipper)
   {
      if(!(m_d.m_OverridePhysics || (m_ptable->m_overridePhysicsFlipper && m_ptable->m_overridePhysics)))
         m_phitflipper->m_elasticityFalloff = newVal;
   }
   else
   {
      STARTUNDO
      m_d.m_elasticityFalloff = newVal;
      STOPUNDO
   }

   return S_OK;
}

STDMETHODIMP Flipper::get_Friction(float *pVal)
{
   *pVal = m_phitflipper ? m_phitflipper->m_friction : m_d.m_friction;

   return S_OK;
}

STDMETHODIMP Flipper::put_Friction(float newVal)
{
   if (m_phitflipper)
   {
      m_phitflipper->SetFriction(newVal);
   }
   else
   {
      STARTUNDO
      m_d.m_friction = newVal;
      STOPUNDO
   }

   return S_OK;
}

STDMETHODIMP Flipper::get_RampUp(float *pVal)
{
   *pVal = (m_d.m_OverridePhysics || (m_ptable->m_overridePhysicsFlipper && m_ptable->m_overridePhysics)) ? m_d.m_OverrideCoilRampUp : m_d.m_rampUp;
   return S_OK;
}

STDMETHODIMP Flipper::put_RampUp(float newVal)
{
   if (m_phitflipper)
   {
      if(!(m_d.m_OverridePhysics || (m_ptable->m_overridePhysicsFlipper && m_ptable->m_overridePhysics)))
         m_d.m_rampUp = newVal;
   }
   else
   {
      STARTUNDO
      m_d.m_rampUp = newVal;
      STOPUNDO
   }
   
   return S_OK;
}

STDMETHODIMP Flipper::get_Height(float *pVal)
{
   *pVal = m_d.m_height;
   UpdateUnitsInfo();
   return S_OK;
}

STDMETHODIMP Flipper::put_Height(float newVal)
{
   STARTUNDO

   m_d.m_height = newVal;
   UpdateUnitsInfo();

   STOPUNDO

   return S_OK;
}

STDMETHODIMP Flipper::get_Return(float *pVal)
{
   if (m_phitflipper)
   {
       *pVal = m_phitflipper->m_flipperMover.GetReturnRatio();
   }
   else
       *pVal = m_d.m_return;

   return S_OK;
}

STDMETHODIMP Flipper::put_Return(float newVal)
{
   if (m_phitflipper)
   {
      if(!(m_d.m_OverridePhysics || (m_ptable->m_overridePhysicsFlipper && m_ptable->m_overridePhysics)))
         m_d.m_return = clamp(newVal, 0.0f, 1.0f);
   }
   else
   {
      STARTUNDO

      m_d.m_return = clamp(newVal, 0.0f, 1.0f);

      STOPUNDO
   }

   return S_OK;
}

STDMETHODIMP Flipper::get_FlipperRadiusMin(float *pVal)
{
   *pVal = m_d.m_FlipperRadiusMin;

   return S_OK;
}

STDMETHODIMP Flipper::put_FlipperRadiusMin(float newVal)
{
   if (newVal < 0.0f) newVal = 0.0f;

   STARTUNDO

   m_d.m_FlipperRadiusMin = newVal;

   STOPUNDO

   return S_OK;
}

STDMETHODIMP Flipper::get_Image(BSTR *pVal)
{
   WCHAR wz[512];

   MultiByteToWideChar(CP_ACP, 0, m_d.m_szImage, -1, wz, 32);
   *pVal = SysAllocString(wz);

   return S_OK;
}

STDMETHODIMP Flipper::put_Image(BSTR newVal)
{
   char szImage[MAXTOKEN];
   WideCharToMultiByte(CP_ACP, 0, newVal, -1, szImage, 32, NULL, NULL);
   const Texture * const tex = m_ptable->GetImage(szImage);
   if(tex && tex->IsHDR())
   {
       ShowError("Cannot use a HDR image (.exr/.hdr) here");
       return E_FAIL;
   }

   STARTUNDO

   strcpy_s(m_d.m_szImage,szImage);
   
   STOPUNDO

   return S_OK;
}

STDMETHODIMP Flipper::get_ReflectionEnabled(VARIANT_BOOL *pVal)
{
   *pVal = (VARIANT_BOOL)FTOVB(m_d.m_fReflectionEnabled);

   return S_OK;
}

STDMETHODIMP Flipper::put_ReflectionEnabled(VARIANT_BOOL newVal)
{
   STARTUNDO

   m_d.m_fReflectionEnabled = VBTOF(newVal);

   STOPUNDO

   return S_OK;
}
