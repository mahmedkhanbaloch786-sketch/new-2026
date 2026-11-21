// ColorSuite OFX image effect — functional CPU rendering implementation.
// Requires the official OpenFX SDK headers and a compatible OFX host/bridge.
#include <algorithm>
#include <cmath>
#include <cstring>
#include <string>
#include <vector>
#include "ofxCore.h"
#include "ofxImageEffect.h"
#include "ofxParam.h"

static OfxHost* gHost=nullptr;
static OfxImageEffectSuiteV1* gEffect=nullptr;
static OfxPropertySuiteV1* gProp=nullptr;
static OfxParameterSuiteV1* gParam=nullptr;
static constexpr const char* ID="com.colorsuite.edius.ColorSuite";
static constexpr double PI=3.14159265358979323846;
static void ss(OfxPropertySetHandle p,const char*k,const char*v){if(gProp&&p)gProp->propSetString(p,k,0,v);}
static void si(OfxPropertySetHandle p,const char*k,int v){if(gProp&&p)gProp->propSetInt(p,k,0,v);}
static void sd(OfxPropertySetHandle p,const char*k,double v){if(gProp&&p)gProp->propSetDouble(p,k,0,v);}
static OfxStatus fetch(OfxHost*h){if(!h||!h->fetchSuite)return kOfxStatErrMissingHostFeature;gHost=h;gEffect=(OfxImageEffectSuiteV1*)h->fetchSuite(h->host,kOfxImageEffectSuite,1);gProp=(OfxPropertySuiteV1*)h->fetchSuite(h->host,kOfxPropertySuite,1);gParam=(OfxParameterSuiteV1*)h->fetchSuite(h->host,kOfxParameterSuite,1);return(gEffect&&gProp&&gParam)?kOfxStatOK:kOfxStatErrMissingHostFeature;}
static void dparam(OfxParamSetHandle ps,const char*n,const char*l,double def,double mn,double mx,const char*hint){OfxPropertySetHandle p=nullptr;if(gParam->paramDefine(ps,kOfxParamTypeDouble,n,&p)!=kOfxStatOK)return;ss(p,kOfxPropLabel,l);sd(p,kOfxParamPropDefault,def);sd(p,kOfxParamPropMin,mn);sd(p,kOfxParamPropMax,mx);ss(p,kOfxParamPropHint,hint);}
static void bparam(OfxParamSetHandle ps,const char*n,const char*l,bool def,const char*hint){OfxPropertySetHandle p=nullptr;if(gParam->paramDefine(ps,kOfxParamTypeBoolean,n,&p)!=kOfxStatOK)return;ss(p,kOfxPropLabel,l);si(p,kOfxParamPropDefault,def?1:0);ss(p,kOfxParamPropHint,hint);}
static void defineParams(OfxParamSetHandle p){
#define D(n,l,d,mn,mx,page) dparam(p,n,l,d,mn,mx,page)
 D("p1_saturation","Saturation",1,0,2,"Page 1 — Basic Adjustments");D("p1_sharpness","Sharpness",0,-1,1,"Page 1 — Basic Adjustments");D("p1_detail","Detail",0,-1,1,"Page 1 — Basic Adjustments");D("p1_contrast","Contrast",1,0,2,"Page 1 — Basic Adjustments");D("p1_exposure","Exposure",0,-4,4,"Page 1 — Basic Adjustments");D("p1_blend","Global Blend",1,0,1,"Page 1 — Basic Adjustments");
 const char* mn[9]={"p2_rr","p2_rg","p2_rb","p2_gr","p2_gg","p2_gb","p2_br","p2_bg","p2_bb"};const char* ml[9]={"Red from Red","Red from Green","Red from Blue","Green from Red","Green from Green","Green from Blue","Blue from Red","Blue from Green","Blue from Blue"};for(int i=0;i<9;i++)D(mn[i],ml[i],(i==0||i==4||i==8)?1:0,-2,2,"Page 2 — Color Matrix");D("p2_blend","Matrix Blend",1,0,1,"Page 2 — Color Matrix");
 D("p3_red","Red Saturation",1,0,3,"Page 3 — Color Saturation");D("p3_green","Green Saturation",1,0,3,"Page 3 — Color Saturation");D("p3_blue","Blue Saturation",1,0,3,"Page 3 — Color Saturation");D("p3_cyan","Cyan Saturation",1,0,3,"Page 3 — Color Saturation");D("p3_magenta","Magenta Saturation",1,0,3,"Page 3 — Color Saturation");D("p3_yellow","Yellow Saturation",1,0,3,"Page 3 — Color Saturation");D("p3_global","Global Saturation",1,0,3,"Page 3 — Color Saturation");
 D("p4_hue_center","Hue Center",0,0,360,"Page 4 — Beauty & Color Selection");D("p4_hue_width","Hue Width",180,1,180,"Page 4 — Beauty & Color Selection");D("p4_sat_min","Saturation Minimum",0,0,1,"Page 4 — Beauty & Color Selection");D("p4_sat_max","Saturation Maximum",1,0,1,"Page 4 — Beauty & Color Selection");D("p4_luma_min","Luma Minimum",0,0,1,"Page 4 — Beauty & Color Selection");D("p4_luma_max","Luma Maximum",1,0,1,"Page 4 — Beauty & Color Selection");D("p4_dark","Dark Selection",0,0,1,"Page 4 — Beauty & Color Selection");D("p4_white","White Selection",0,0,1,"Page 4 — Beauty & Color Selection");D("p4_black","Black Selection",0,0,1,"Page 4 — Beauty & Color Selection");D("p4_red","Red Selection",0,0,1,"Page 4 — Beauty & Color Selection");bparam(p,"p4_mask","Show Selection Mask",false,"Page 4 — Beauty & Color Selection");
 D("p5_skin_range","Skin Range",0.5,0,1,"Page 5 — Skin Retouching");D("p5_sharpness","Skin Sharpness",0,-1,1,"Page 5 — Skin Retouching");D("p5_detail","Skin Detail",0,-1,1,"Page 5 — Skin Retouching");D("p5_blur","Blur",0,0,1,"Page 5 — Skin Retouching");D("p5_softness","Softness",0,0,1,"Page 5 — Skin Retouching");D("p5_strength","Retouch Strength",0,0,1,"Page 5 — Skin Retouching");
 D("p6_red_gain","Red Gain",1,0,3,"Page 6 — Advanced Controls");D("p6_green_gain","Green Gain",1,0,3,"Page 6 — Advanced Controls");D("p6_blue_gain","Blue Gain",1,0,3,"Page 6 — Advanced Controls");D("p6_red_offset","Red Offset",0,-.5,.5,"Page 6 — Advanced Controls");D("p6_green_offset","Green Offset",0,-.5,.5,"Page 6 — Advanced Controls");D("p6_blue_offset","Blue Offset",0,-.5,.5,"Page 6 — Advanced Controls");D("p6_hue","Hue Rotation",0,-180,180,"Page 6 — Advanced Controls");D("p6_luma","Luma",0,-1,1,"Page 6 — Advanced Controls");D("p6_blend","Global Blend",1,0,1,"Page 6 — Advanced Controls");
#undef D
}
static double val(OfxParamSetHandle ps,const char*n,double def,double t){OfxParamHandle h=nullptr;double v=def;if(gParam->paramGetHandle(ps,n,&h,nullptr)==kOfxStatOK&&h)gParam->paramGetValueAtTime(h,t,&v);return v;}
static bool bval(OfxParamSetHandle ps,const char*n,bool def,double t){OfxParamHandle h=nullptr;int v=def?1:0;if(gParam->paramGetHandle(ps,n,&h,nullptr)==kOfxStatOK&&h)gParam->paramGetValueAtTime(h,t,&v);return v!=0;}
static double clamp01(double x){return std::max(0.0,std::min(1.0,x));}
struct P{double r,g,b,a;};
static void rgbhsv(double r,double g,double b,double&h,double&s,double&v){double mx=std::max({r,g,b}),mn=std::min({r,g,b}),d=mx-mn;v=mx;s=mx<=1e-8?0:d/mx;if(d<1e-8)h=0;else if(mx==r)h=60*std::fmod((g-b)/d+6,6);else if(mx==g)h=60*((b-r)/d+2);else h=60*((r-g)/d+4);}
static P process(P src,const P&blur,OfxParamSetHandle ps,double t){
 double r=src.r,g=src.g,b=src.b,origR=r,origG=g,origB=b;
 double exp=std::pow(2.0,val(ps,"p1_exposure",0,t));r*=exp;g*=exp;b*=exp;
 double contrast=val(ps,"p1_contrast",1,t);r=(r-.18)*contrast+.18;g=(g-.18)*contrast+.18;b=(b-.18)*contrast+.18;
 double rr=val(ps,"p2_rr",1,t),rg=val(ps,"p2_rg",0,t),rb=val(ps,"p2_rb",0,t),gr=val(ps,"p2_gr",0,t),gg=val(ps,"p2_gg",1,t),gb=val(ps,"p2_gb",0,t),br=val(ps,"p2_br",0,t),bg=val(ps,"p2_bg",0,t),bb=val(ps,"p2_bb",1,t),mb=val(ps,"p2_blend",1,t);
 double mr=rr*r+rg*g+rb*b,mg=gr*r+gg*g+gb*b,mbb=br*r+bg*g+bb*b;r=r*(1-mb)+mr*mb;g=g*(1-mb)+mg*mb;b=b*(1-mb)+mbb*mb;
 r=r*val(ps,"p6_red_gain",1,t)+val(ps,"p6_red_offset",0,t);g=g*val(ps,"p6_green_gain",1,t)+val(ps,"p6_green_offset",0,t);b=b*val(ps,"p6_blue_gain",1,t)+val(ps,"p6_blue_offset",0,t);
 double h,s,v;rgbhsv(r,g,b,h,s,v);double sat=val(ps,"p1_saturation",1,t)*val(ps,"p3_global",1,t);
 // Smooth hue-region weighting for the six color saturation controls.
 double hues[6]={0,120,240,180,300,60};const char* keys[6]={"p3_red","p3_green","p3_blue","p3_cyan","p3_magenta","p3_yellow"};for(int i=0;i<6;i++){double d=std::fabs(std::fmod(h-hues[i]+540,360)-180);double w=clamp01(1.0-d/55.0);sat*=1.0+w*(val(ps,keys[i],1,t)-1.0);}
 s=clamp01(s*sat);double ang=(h+val(ps,"p6_hue",0,t));ang=std::fmod(ang+360,360);double c=v*s,x=c*(1-std::fabs(std::fmod(ang/60.0,2.0)-1)),m=v-c;double nr=0,ng=0,nb=0;if(ang<60){nr=c;ng=x;}else if(ang<120){nr=x;ng=c;}else if(ang<180){ng=c;nb=x;}else if(ang<240){ng=x;nb=c;}else if(ang<300){nr=x;nb=c;}else{nr=c;nb=x;}r=nr+m;g=ng+m;b=nb+m;
 double lum=.2126*r+.7152*g+.0722*b;lum+=val(ps,"p6_luma",0,t);r+=(lum-(.2126*r+.7152*g+.0722*b));g+=(lum-(.2126*r+.7152*g+.0722*b));b+=(lum-(.2126*r+.7152*g+.0722*b));
 // Approximate skin selection using warm hue and saturation/luma ranges. Spatial smoothing uses 3x3 blur.
 double sh,ssat,sv;rgbhsv(clamp01(r),clamp01(g),clamp01(b),sh,ssat,sv);double hc=val(ps,"p4_hue_center",25,t),hw=val(ps,"p4_hue_width",180,t);double hd=std::fabs(std::fmod(sh-hc+540,360)-180);double select=clamp01(1-hd/std::max(1.0,hw))*clamp01((ssat-val(ps,"p4_sat_min",0,t))/std::max(.001,val(ps,"p4_sat_max",1,t)-val(ps,"p4_sat_min",0,t)))*clamp01((sv-val(ps,"p4_luma_min",0,t))/std::max(.001,val(ps,"p4_luma_max",1,t)-val(ps,"p4_luma_min",0,t)));
 select=std::max(select,val(ps,"p4_red",0,t)*clamp01((r-g)*2));select=clamp01(select*val(ps,"p5_skin_range",.5,t)*2);
 double strength=clamp01(val(ps,"p5_strength",0,t)+val(ps,"p5_blur",0,t)*.5+val(ps,"p5_softness",0,t)*.5)*select;
 r=r*(1-strength)+blur.r*strength;g=g*(1-strength)+blur.g*strength;b=b*(1-strength)+blur.b*strength;
 double detail=val(ps,"p1_detail",0,t)+val(ps,"p5_detail",0,t)*select;double sharp=val(ps,"p1_sharpness",0,t)+val(ps,"p5_sharpness",0,t)*select;double brt=blur.r,bgt=blur.g,bbt=blur.b;r+=((r-brt)*detail)+((r-brt)*sharp);g+=((g-bgt)*detail)+((g-bgt)*sharp);b+=((b-bbt)*detail)+((b-bbt)*sharp);
 if(bval(ps,"p4_mask",false,t)){r=select;g=select;b=select;}
 double blend=clamp01(val(ps,"p1_blend",1,t)*val(ps,"p6_blend",1,t));return {origR*(1-blend)+r*blend,origG*(1-blend)+g*blend,origB*(1-blend)+b*blend,src.a};
}
static OfxStatus describe(OfxImageEffectHandle e){OfxPropertySetHandle p=nullptr;if(gEffect->getPropertySet(e,&p)!=kOfxStatOK)return kOfxStatErrUnknown;ss(p,kOfxPropLabel,"ColorSuite — Color & Beauty");ss(p,kOfxImageEffectPropSupportedContexts,kOfxImageEffectContextFilter);ss(p,kOfxImageEffectPropSupportedPixelDepths,kOfxBitDepthFloat);si(p,kOfxImageEffectPropSupportsTiles,0);si(p,kOfxImageEffectPropSupportsMultiResolution,1);return kOfxStatOK;}
static OfxStatus describeContext(OfxImageEffectHandle e){OfxPropertySetHandle p=nullptr;if(gEffect->clipDefine(e,kOfxImageEffectSimpleSourceClipName,&p)!=kOfxStatOK)return kOfxStatErrUnknown;ss(p,kOfxPropLabel,"Source");if(gEffect->clipDefine(e,kOfxImageEffectOutputClipName,&p)!=kOfxStatOK)return kOfxStatErrUnknown;ss(p,kOfxPropLabel,"Output");OfxParamSetHandle ps=nullptr;if(gEffect->getParamSet(e,&ps)!=kOfxStatOK)return kOfxStatErrUnknown;defineParams(ps);return kOfxStatOK;}
static OfxStatus render(OfxImageEffectHandle e,OfxPropertySetHandle args){double time=0;gProp->propGetDouble(args,kOfxPropTime,0,&time);OfxPropertySetHandle srcP=nullptr,outP=nullptr;OfxImageClipHandle srcC=nullptr,outC=nullptr;if(gEffect->clipGetHandle(e,kOfxImageEffectSimpleSourceClipName,&srcC,&srcP)!=kOfxStatOK||gEffect->clipGetHandle(e,kOfxImageEffectOutputClipName,&outC,&outP)!=kOfxStatOK)return kOfxStatErrUnknown;OfxPropertySetHandle in=nullptr,out=nullptr;if(gEffect->clipGetImage(srcC,time,nullptr,&in)!=kOfxStatOK||!in)return kOfxStatFailed;if(gEffect->clipGetImage(outC,time,nullptr,&out)!=kOfxStatOK||!out){gEffect->clipReleaseImage(in);return kOfxStatFailed;}
 void *srcData=nullptr,*dstData=nullptr;int sx=0,sy=0,dx=0,dy=0;gProp->propGetPointer(in,kOfxImagePropData,0,&srcData);gProp->propGetPointer(out,kOfxImagePropData,0,&dstData);gProp->propGetInt(in,kOfxImagePropRowBytes,0,&sx);gProp->propGetInt(out,kOfxImagePropRowBytes,0,&sy);int sb[4]={0},db[4]={0};gProp->propGetInt(in,kOfxImagePropBounds,0,&sb[0]);gProp->propGetInt(in,kOfxImagePropBounds,1,&sb[1]);gProp->propGetInt(in,kOfxImagePropBounds,2,&sb[2]);gProp->propGetInt(in,kOfxImagePropBounds,3,&sb[3]);gProp->propGetInt(out,kOfxImagePropBounds,0,&db[0]);gProp->propGetInt(out,kOfxImagePropBounds,1,&db[1]);gProp->propGetInt(out,kOfxImagePropBounds,2,&db[2]);gProp->propGetInt(out,kOfxImagePropBounds,3,&db[3]);OfxParamSetHandle ps=nullptr;gEffect->getParamSet(e,&ps);
 if(!srcData||!dstData){gEffect->clipReleaseImage(in);gEffect->clipReleaseImage(out);return kOfxStatFailed;}int w=std::min(sb[2]-sb[0],db[2]-db[0]),h=std::min(sb[3]-sb[1],db[3]-db[1]);if(w<=0||h<=0){gEffect->clipReleaseImage(in);gEffect->clipReleaseImage(out);return kOfxStatOK;}auto*base=(unsigned char*)srcData;auto*dest=(unsigned char*)dstData;auto pixel=[&](int x,int y)->P{int xx=std::max(0,std::min(w-1,x)),yy=std::max(0,std::min(h-1,y));float*q=(float*)(base+(size_t)yy*sx+(size_t)xx*4*sizeof(float));return{q[0],q[1],q[2],q[3]};};
 for(int y=0;y<h;y++){float*row=(float*)(dest+(size_t)y*sy);for(int x=0;x<w;x++){P a=pixel(x,y),blur{0,0,0,0};for(int oy=-1;oy<=1;oy++)for(int ox=-1;ox<=1;ox++){P q=pixel(x+ox,y+oy);blur.r+=q.r/9;blur.g+=q.g/9;blur.b+=q.b/9;blur.a+=q.a/9;}P z=process(a,blur,ps,time);row[x*4+0]=(float)z.r;row[x*4+1]=(float)z.g;row[x*4+2]=(float)z.b;row[x*4+3]=(float)z.a;}}
 gEffect->clipReleaseImage(in);gEffect->clipReleaseImage(out);return kOfxStatOK;}
static OfxStatus mainEntry(const char*a,const void*h,OfxPropertySetHandle in,OfxPropertySetHandle out){(void)out;auto e=(OfxImageEffectHandle)h;if(!std::strcmp(a,kOfxActionLoad)||!std::strcmp(a,kOfxActionUnload))return kOfxStatOK;if(!std::strcmp(a,kOfxActionDescribe))return describe(e);if(!std::strcmp(a,kOfxImageEffectActionDescribeInContext))return describeContext(e);if(!std::strcmp(a,kOfxImageEffectActionRender))return render(e,in);return kOfxStatReplyDefault;}
static OfxPlugin plugin={kOfxImageEffectPluginApi,1,ID,1,0,nullptr,mainEntry};
extern "C" {OfxExport int OfxGetNumberOfPlugins(void){return 1;}OfxExport OfxPlugin* OfxGetPlugin(int i){if(i)return nullptr;plugin.setHost=[](OfxHost*h){fetch(h);};return &plugin;}}
