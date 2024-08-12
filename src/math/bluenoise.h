// license:GPLv3+

// precomputed blue-noise'ish points for oversampling the static pre-rendered objects

#define STATIC_PRERENDER_ITERATIONS /*32*//*64*/128
/*   static const float xyLDBNbnot[STATIC_PRERENDER_ITERATIONS*2] = {
     0.00000f,0.00000f,
     0.23151f,0.02134f,
     0.36499f,0.09920f,
     0.72093f,0.06583f,
     0.10915f,0.14369f,
     0.28712f,0.25492f,
     0.56520f,0.15482f,
     0.79879f,0.23268f,
     0.97676f,0.14925f,
     0.14252f,0.35503f,
     0.34274f,0.44402f,
     0.46509f,0.31054f,
     0.65419f,0.33279f,
     0.83216f,0.41621f,
     0.98788f,0.34947f,
     0.00905f,0.46626f,
     0.20926f,0.52188f,
     0.38723f,0.62199f,
     0.54296f,0.47739f,
     0.66531f,0.57749f,
     0.82103f,0.60530f,
     0.07578f,0.64423f,
     0.13140f,0.79996f,
     0.27600f,0.72209f,
     0.53183f,0.68873f,
     0.68756f,0.77771f,
     0.96563f,0.69429f,
     0.17589f,0.97793f,
     0.32049f,0.92231f,
     0.48734f,0.85557f,
     0.67643f,0.95568f,
     0.85440f,0.82776f};*/

/*   static const float xyLDBNbnot[STATIC_PRERENDER_ITERATIONS*2] = {
     0.00000f,0.00000f,
     0.13926f,0.00971f,
     0.99043f,0.01365f,
     0.02105f,0.06487f,
     0.11562f,0.13580f,
     0.24960f,0.14369f,
     0.31265f,0.05699f,
     0.43087f,0.12004f,
     0.56484f,0.11216f,
     0.71459f,0.07670f,
     0.85645f,0.10822f,
     0.96678f,0.14763f,
     0.06045f,0.26978f,
     0.19443f,0.26190f,
     0.36782f,0.22250f,
     0.50968f,0.23826f,
     0.66730f,0.19097f,
     0.78552f,0.19491f,
     0.86433f,0.28161f,
     0.02893f,0.37224f,
     0.15503f,0.39588f,
     0.28900f,0.34071f,
     0.41510f,0.34860f,
     0.52544f,0.36436f,
     0.64366f,0.30131f,
     0.75399f,0.34465f,
     0.88797f,0.41558f,
     0.97466f,0.30525f,
     0.07621f,0.50622f,
     0.21019f,0.49834f,
     0.33629f,0.45105f,
     0.46239f,0.49046f,
     0.62789f,0.44317f,
     0.76975f,0.47075f,
     0.91161f,0.54168f,
     0.12350f,0.62443f,
     0.28112f,0.58503f,
     0.39146f,0.60867f,
     0.54908f,0.56139f,
     0.60425f,0.65596f,
     0.69094f,0.55745f,
     0.82492f,0.61261f,
     0.09001f,0.75841f,
     0.20822f,0.71113f,
     0.32644f,0.72689f,
     0.49194f,0.68748f,
     0.62592f,0.78994f,
     0.72838f,0.70719f,
     0.83871f,0.76235f,
     0.94905f,0.69142f,
     0.00331f,0.87663f,
     0.09789f,0.90816f,
     0.21610f,0.83723f,
     0.38949f,0.82146f,
     0.50771f,0.81358f,
     0.57864f,0.90027f,
     0.74414f,0.85693f,
     0.85448f,0.89633f,
     0.97269f,0.81752f,
     0.27915f,0.92392f,
     0.35008f,0.99485f,
     0.45254f,0.96332f,
     0.69685f,0.96726f,
     0.91753f,0.97515f};*/

   static const float xyLDBNbnot[STATIC_PRERENDER_ITERATIONS*2] = {
     0.00000f,0.00000f,
     0.01267f,0.07171f,
     0.10232f,0.04930f,
     0.18638f,0.01007f,
     0.28725f,0.02688f,
     0.43295f,0.08292f,
     0.50579f,0.00447f,
     0.53942f,0.07732f,
     0.66830f,0.03249f,
     0.75796f,0.06611f,
     0.88685f,0.01568f,
     0.98211f,0.05490f,
     0.07431f,0.14456f,
     0.15836f,0.13335f,
     0.22000f,0.09413f,
     0.30406f,0.13896f,
     0.36010f,0.08852f,
     0.52260f,0.16137f,
     0.61226f,0.12775f,
     0.69632f,0.11654f,
     0.78598f,0.17258f,
     0.84202f,0.09973f,
     0.91487f,0.11094f,
     0.99332f,0.15577f,
     0.01827f,0.18379f,
     0.13034f,0.23422f,
     0.23681f,0.18939f,
     0.33208f,0.22301f,
     0.41613f,0.17818f,
     0.48898f,0.22862f,
     0.59545f,0.24543f,
     0.67951f,0.20060f,
     0.77477f,0.25103f,
     0.88124f,0.19499f,
     0.97090f,0.23982f,
     0.04629f,0.27345f,
     0.08551f,0.34630f,
     0.19759f,0.26784f,
     0.25923f,0.31828f,
     0.35449f,0.30707f,
     0.43855f,0.29586f,
     0.52821f,0.31267f,
     0.62347f,0.33509f,
     0.70753f,0.28465f,
     0.80840f,0.32388f,
     0.89245f,0.27905f,
     0.99892f,0.32948f,
     0.06310f,0.42475f,
     0.16397f,0.35750f,
     0.24242f,0.43035f,
     0.32087f,0.39113f,
     0.41053f,0.38552f,
     0.47778f,0.41354f,
     0.56743f,0.39673f,
     0.64589f,0.43596f,
     0.71313f,0.37431f,
     0.81400f,0.40794f,
     0.90366f,0.36871f,
     0.97651f,0.41914f,
     0.00006f,0.45837f,
     0.10653f,0.52001f,
     0.16817f,0.45277f,
     0.29145f,0.51441f,
     0.35870f,0.48079f,
     0.44836f,0.50320f,
     0.54362f,0.49199f,
     0.62768f,0.53122f,
     0.73975f,0.46397f,
     0.87424f,0.47518f,
     0.95269f,0.49760f,
     0.01687f,0.54803f,
     0.16257f,0.61527f,
     0.20179f,0.54243f,
     0.26904f,0.60967f,
     0.36430f,0.58165f,
     0.46517f,0.59286f,
     0.54922f,0.58726f,
     0.70052f,0.55363f,
     0.79018f,0.53682f,
     0.80139f,0.62088f,
     0.88545f,0.57044f,
     0.07291f,0.70493f,
     0.07851f,0.62648f,
     0.20739f,0.68252f,
     0.30826f,0.68812f,
     0.39792f,0.66571f,
     0.49319f,0.67131f,
     0.60526f,0.64890f,
     0.69492f,0.63209f,
     0.77337f,0.71054f,
     0.86303f,0.69933f,
     0.93028f,0.63769f,
     0.01126f,0.73856f,
     0.14575f,0.76097f,
     0.24102f,0.77218f,
     0.31387f,0.79459f,
     0.40353f,0.74976f,
     0.48758f,0.76658f,
     0.57724f,0.72735f,
     0.68371f,0.72175f,
     0.72294f,0.80020f,
     0.82941f,0.77778f,
     0.93588f,0.75537f,
     0.06730f,0.81701f,
     0.11774f,0.88986f,
     0.19058f,0.85624f,
     0.28024f,0.88425f,
     0.38671f,0.83942f,
     0.47637f,0.86184f,
     0.56603f,0.83382f,
     0.63888f,0.80580f,
     0.76777f,0.87305f,
     0.84622f,0.86744f,
     0.91347f,0.84503f,
     0.02808f,0.90667f,
     0.10092f,0.97952f,
     0.18498f,0.96831f,
     0.26343f,0.97391f,
     0.36990f,0.92348f,
     0.45956f,0.94589f,
     0.55483f,0.91788f,
     0.65009f,0.89546f,
     0.70613f,0.95710f,
     0.80699f,0.96271f,
     0.90786f,0.95150f,
     0.99192f,0.91227f,
     0.00566f,0.99072f,
     0.61086f,0.98512f};
