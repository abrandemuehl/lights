<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="8.5.2">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="2" name="Route2" color="1" fill="3" visible="no" active="no"/>
<layer number="3" name="Route3" color="4" fill="3" visible="no" active="no"/>
<layer number="4" name="Route4" color="1" fill="4" visible="no" active="no"/>
<layer number="5" name="Route5" color="4" fill="4" visible="no" active="no"/>
<layer number="6" name="Route6" color="1" fill="8" visible="no" active="no"/>
<layer number="7" name="Route7" color="4" fill="8" visible="no" active="no"/>
<layer number="8" name="Route8" color="1" fill="2" visible="no" active="no"/>
<layer number="9" name="Route9" color="4" fill="2" visible="no" active="no"/>
<layer number="10" name="Route10" color="1" fill="7" visible="no" active="no"/>
<layer number="11" name="Route11" color="4" fill="7" visible="no" active="no"/>
<layer number="12" name="Route12" color="1" fill="5" visible="no" active="no"/>
<layer number="13" name="Route13" color="4" fill="5" visible="no" active="no"/>
<layer number="14" name="Route14" color="1" fill="6" visible="no" active="no"/>
<layer number="15" name="Route15" color="4" fill="6" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="24" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="88" name="SimResults" color="9" fill="1" visible="yes" active="yes"/>
<layer number="89" name="SimProbes" color="9" fill="1" visible="yes" active="yes"/>
<layer number="90" name="Modules" color="5" fill="1" visible="yes" active="yes"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="lights">
<packages>
<package name="STM32F030CC">
<description>&lt;b&gt;48-pin plastic LQFP (FPT-48P-M26)&lt;/b&gt;&lt;p&gt;
www.fma.fujitsu.com/pdf/e713717.pdf</description>
<wire x1="-3.375" y1="3.1" x2="-3.1" y2="3.375" width="0.254" layer="21"/>
<wire x1="-3.1" y1="3.375" x2="3.1" y2="3.375" width="0.254" layer="21"/>
<wire x1="3.1" y1="3.375" x2="3.375" y2="3.1" width="0.254" layer="21"/>
<wire x1="3.375" y1="3.1" x2="3.375" y2="-3.1" width="0.254" layer="21"/>
<wire x1="3.375" y1="-3.1" x2="3.1" y2="-3.375" width="0.254" layer="21"/>
<wire x1="3.1" y1="-3.375" x2="-3.1" y2="-3.375" width="0.254" layer="21"/>
<wire x1="-3.1" y1="-3.375" x2="-3.375" y2="-3.1" width="0.254" layer="21"/>
<wire x1="-3.375" y1="-3.1" x2="-3.375" y2="3.1" width="0.254" layer="21"/>
<circle x="-2" y="-2" radius="0.6" width="0.254" layer="21"/>
<smd name="1" x="-2.75" y="-4.25" dx="0.2" dy="1" layer="1"/>
<smd name="2" x="-2.25" y="-4.25" dx="0.2" dy="1" layer="1"/>
<smd name="3" x="-1.75" y="-4.25" dx="0.2" dy="1" layer="1"/>
<smd name="4" x="-1.25" y="-4.25" dx="0.2" dy="1" layer="1"/>
<smd name="5" x="-0.75" y="-4.25" dx="0.2" dy="1" layer="1"/>
<smd name="6" x="-0.25" y="-4.25" dx="0.2" dy="1" layer="1"/>
<smd name="7" x="0.25" y="-4.25" dx="0.2" dy="1" layer="1"/>
<smd name="8" x="0.75" y="-4.25" dx="0.2" dy="1" layer="1"/>
<smd name="9" x="1.25" y="-4.25" dx="0.2" dy="1" layer="1"/>
<smd name="10" x="1.75" y="-4.25" dx="0.2" dy="1" layer="1"/>
<smd name="11" x="2.25" y="-4.25" dx="0.2" dy="1" layer="1"/>
<smd name="12" x="2.75" y="-4.25" dx="0.2" dy="1" layer="1"/>
<smd name="13" x="4.25" y="-2.75" dx="1" dy="0.2" layer="1"/>
<smd name="14" x="4.25" y="-2.25" dx="1" dy="0.2" layer="1"/>
<smd name="15" x="4.25" y="-1.75" dx="1" dy="0.2" layer="1"/>
<smd name="16" x="4.25" y="-1.25" dx="1" dy="0.2" layer="1"/>
<smd name="17" x="4.25" y="-0.75" dx="1" dy="0.2" layer="1"/>
<smd name="18" x="4.25" y="-0.25" dx="1" dy="0.2" layer="1"/>
<smd name="19" x="4.25" y="0.25" dx="1" dy="0.2" layer="1"/>
<smd name="20" x="4.25" y="0.75" dx="1" dy="0.2" layer="1"/>
<smd name="21" x="4.25" y="1.25" dx="1" dy="0.2" layer="1"/>
<smd name="22" x="4.25" y="1.75" dx="1" dy="0.2" layer="1"/>
<smd name="23" x="4.25" y="2.25" dx="1" dy="0.2" layer="1"/>
<smd name="24" x="4.25" y="2.75" dx="1" dy="0.2" layer="1"/>
<smd name="25" x="2.75" y="4.25" dx="0.2" dy="1" layer="1"/>
<smd name="26" x="2.25" y="4.25" dx="0.2" dy="1" layer="1"/>
<smd name="27" x="1.75" y="4.25" dx="0.2" dy="1" layer="1"/>
<smd name="28" x="1.25" y="4.25" dx="0.2" dy="1" layer="1"/>
<smd name="29" x="0.75" y="4.25" dx="0.2" dy="1" layer="1"/>
<smd name="30" x="0.25" y="4.25" dx="0.2" dy="1" layer="1"/>
<smd name="31" x="-0.25" y="4.25" dx="0.2" dy="1" layer="1"/>
<smd name="32" x="-0.75" y="4.25" dx="0.2" dy="1" layer="1"/>
<smd name="33" x="-1.25" y="4.25" dx="0.2" dy="1" layer="1"/>
<smd name="34" x="-1.75" y="4.25" dx="0.2" dy="1" layer="1"/>
<smd name="35" x="-2.25" y="4.25" dx="0.2" dy="1" layer="1"/>
<smd name="36" x="-2.75" y="4.25" dx="0.2" dy="1" layer="1"/>
<smd name="37" x="-4.25" y="2.75" dx="1" dy="0.2" layer="1"/>
<smd name="38" x="-4.25" y="2.25" dx="1" dy="0.2" layer="1"/>
<smd name="39" x="-4.25" y="1.75" dx="1" dy="0.2" layer="1"/>
<smd name="40" x="-4.25" y="1.25" dx="1" dy="0.2" layer="1"/>
<smd name="41" x="-4.25" y="0.75" dx="1" dy="0.2" layer="1"/>
<smd name="42" x="-4.25" y="0.25" dx="1" dy="0.2" layer="1"/>
<smd name="43" x="-4.25" y="-0.25" dx="1" dy="0.2" layer="1"/>
<smd name="44" x="-4.25" y="-0.75" dx="1" dy="0.2" layer="1"/>
<smd name="45" x="-4.25" y="-1.25" dx="1" dy="0.2" layer="1"/>
<smd name="46" x="-4.25" y="-1.75" dx="1" dy="0.2" layer="1"/>
<smd name="47" x="-4.25" y="-2.25" dx="1" dy="0.2" layer="1"/>
<smd name="48" x="-4.25" y="-2.75" dx="1" dy="0.2" layer="1"/>
<text x="-2.54" y="1.27" size="1.27" layer="25">&gt;NAME</text>
<text x="-2.54" y="-1.27" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-2.85" y1="-4.5" x2="-2.65" y2="-3.45" layer="51"/>
<rectangle x1="-2.35" y1="-4.5" x2="-2.15" y2="-3.45" layer="51"/>
<rectangle x1="-1.85" y1="-4.5" x2="-1.65" y2="-3.45" layer="51"/>
<rectangle x1="-1.35" y1="-4.5" x2="-1.15" y2="-3.45" layer="51"/>
<rectangle x1="-0.85" y1="-4.5" x2="-0.65" y2="-3.45" layer="51"/>
<rectangle x1="-0.35" y1="-4.5" x2="-0.15" y2="-3.45" layer="51"/>
<rectangle x1="0.15" y1="-4.5" x2="0.35" y2="-3.45" layer="51"/>
<rectangle x1="0.65" y1="-4.5" x2="0.85" y2="-3.45" layer="51"/>
<rectangle x1="1.15" y1="-4.5" x2="1.35" y2="-3.45" layer="51"/>
<rectangle x1="1.65" y1="-4.5" x2="1.85" y2="-3.45" layer="51"/>
<rectangle x1="2.15" y1="-4.5" x2="2.35" y2="-3.45" layer="51"/>
<rectangle x1="2.65" y1="-4.5" x2="2.85" y2="-3.45" layer="51"/>
<rectangle x1="3.45" y1="-2.85" x2="4.5" y2="-2.65" layer="51"/>
<rectangle x1="3.45" y1="-2.35" x2="4.5" y2="-2.15" layer="51"/>
<rectangle x1="3.45" y1="-1.85" x2="4.5" y2="-1.65" layer="51"/>
<rectangle x1="3.45" y1="-1.35" x2="4.5" y2="-1.15" layer="51"/>
<rectangle x1="3.45" y1="-0.85" x2="4.5" y2="-0.65" layer="51"/>
<rectangle x1="3.45" y1="-0.35" x2="4.5" y2="-0.15" layer="51"/>
<rectangle x1="3.45" y1="0.15" x2="4.5" y2="0.35" layer="51"/>
<rectangle x1="3.45" y1="0.65" x2="4.5" y2="0.85" layer="51"/>
<rectangle x1="3.45" y1="1.15" x2="4.5" y2="1.35" layer="51"/>
<rectangle x1="3.45" y1="1.65" x2="4.5" y2="1.85" layer="51"/>
<rectangle x1="3.45" y1="2.15" x2="4.5" y2="2.35" layer="51"/>
<rectangle x1="3.45" y1="2.65" x2="4.5" y2="2.85" layer="51"/>
<rectangle x1="2.65" y1="3.45" x2="2.85" y2="4.5" layer="51"/>
<rectangle x1="2.15" y1="3.45" x2="2.35" y2="4.5" layer="51"/>
<rectangle x1="1.65" y1="3.45" x2="1.85" y2="4.5" layer="51"/>
<rectangle x1="1.15" y1="3.45" x2="1.35" y2="4.5" layer="51"/>
<rectangle x1="0.65" y1="3.45" x2="0.85" y2="4.5" layer="51"/>
<rectangle x1="0.15" y1="3.45" x2="0.35" y2="4.5" layer="51"/>
<rectangle x1="-0.35" y1="3.45" x2="-0.15" y2="4.5" layer="51"/>
<rectangle x1="-0.85" y1="3.45" x2="-0.65" y2="4.5" layer="51"/>
<rectangle x1="-1.35" y1="3.45" x2="-1.15" y2="4.5" layer="51"/>
<rectangle x1="-1.85" y1="3.45" x2="-1.65" y2="4.5" layer="51"/>
<rectangle x1="-2.35" y1="3.45" x2="-2.15" y2="4.5" layer="51"/>
<rectangle x1="-2.85" y1="3.45" x2="-2.65" y2="4.5" layer="51"/>
<rectangle x1="-4.5" y1="2.65" x2="-3.45" y2="2.85" layer="51"/>
<rectangle x1="-4.5" y1="2.15" x2="-3.45" y2="2.35" layer="51"/>
<rectangle x1="-4.5" y1="1.65" x2="-3.45" y2="1.85" layer="51"/>
<rectangle x1="-4.5" y1="1.15" x2="-3.45" y2="1.35" layer="51"/>
<rectangle x1="-4.5" y1="0.65" x2="-3.45" y2="0.85" layer="51"/>
<rectangle x1="-4.5" y1="0.15" x2="-3.45" y2="0.35" layer="51"/>
<rectangle x1="-4.5" y1="-0.35" x2="-3.45" y2="-0.15" layer="51"/>
<rectangle x1="-4.5" y1="-0.85" x2="-3.45" y2="-0.65" layer="51"/>
<rectangle x1="-4.5" y1="-1.35" x2="-3.45" y2="-1.15" layer="51"/>
<rectangle x1="-4.5" y1="-1.85" x2="-3.45" y2="-1.65" layer="51"/>
<rectangle x1="-4.5" y1="-2.35" x2="-3.45" y2="-2.15" layer="51"/>
<rectangle x1="-4.5" y1="-2.85" x2="-3.45" y2="-2.65" layer="51"/>
</package>
</packages>
<symbols>
<symbol name="STM32F030CC">
<wire x1="-17.78" y1="30.48" x2="17.78" y2="30.48" width="0.254" layer="94"/>
<wire x1="17.78" y1="30.48" x2="17.78" y2="-33.02" width="0.254" layer="94"/>
<wire x1="17.78" y1="-33.02" x2="-17.78" y2="-33.02" width="0.254" layer="94"/>
<wire x1="-17.78" y1="-33.02" x2="-17.78" y2="30.48" width="0.254" layer="94"/>
<pin name="VDD1" x="-22.86" y="27.94" length="middle"/>
<pin name="PC13" x="-22.86" y="25.4" length="middle"/>
<pin name="PC14-OSC32_IN" x="-22.86" y="22.86" length="middle"/>
<pin name="PC15-OSC32_OUT" x="-22.86" y="20.32" length="middle"/>
<pin name="PF0-OSC_IN" x="-22.86" y="17.78" length="middle"/>
<pin name="PF1-OSC_OUT" x="-22.86" y="15.24" length="middle"/>
<pin name="NRST" x="-22.86" y="12.7" length="middle"/>
<pin name="VSSA" x="-22.86" y="10.16" length="middle"/>
<pin name="VDDA" x="-22.86" y="7.62" length="middle"/>
<pin name="PA0" x="-22.86" y="5.08" length="middle"/>
<pin name="PA1" x="-22.86" y="2.54" length="middle"/>
<pin name="PA2" x="-22.86" y="0" length="middle"/>
<pin name="PA3" x="-22.86" y="-2.54" length="middle"/>
<pin name="PA4" x="-22.86" y="-5.08" length="middle"/>
<pin name="PA5" x="-22.86" y="-7.62" length="middle"/>
<pin name="PA6" x="-22.86" y="-10.16" length="middle"/>
<pin name="PA7" x="-22.86" y="-12.7" length="middle"/>
<pin name="PB0" x="-22.86" y="-15.24" length="middle"/>
<pin name="PB1" x="-22.86" y="-17.78" length="middle"/>
<pin name="PB2" x="-22.86" y="-20.32" length="middle"/>
<pin name="PB10" x="-22.86" y="-22.86" length="middle"/>
<pin name="PB11" x="-22.86" y="-25.4" length="middle"/>
<pin name="VSS23" x="-22.86" y="-27.94" length="middle"/>
<pin name="VDD24" x="-22.86" y="-30.48" length="middle"/>
<pin name="PB12" x="22.86" y="27.94" length="middle" rot="R180"/>
<pin name="PB13" x="22.86" y="25.4" length="middle" rot="R180"/>
<pin name="PB14" x="22.86" y="22.86" length="middle" rot="R180"/>
<pin name="PB15" x="22.86" y="20.32" length="middle" rot="R180"/>
<pin name="PA8" x="22.86" y="17.78" length="middle" rot="R180"/>
<pin name="PA9" x="22.86" y="15.24" length="middle" rot="R180"/>
<pin name="PA10" x="22.86" y="12.7" length="middle" rot="R180"/>
<pin name="PA11" x="22.86" y="10.16" length="middle" rot="R180"/>
<pin name="PA12" x="22.86" y="7.62" length="middle" rot="R180"/>
<pin name="PA13" x="22.86" y="5.08" length="middle" rot="R180"/>
<pin name="VSS35" x="22.86" y="2.54" length="middle" rot="R180"/>
<pin name="VDD36" x="22.86" y="0" length="middle" rot="R180"/>
<pin name="PA14" x="22.86" y="-2.54" length="middle" rot="R180"/>
<pin name="PA15" x="22.86" y="-5.08" length="middle" rot="R180"/>
<pin name="PB3" x="22.86" y="-7.62" length="middle" rot="R180"/>
<pin name="PB4" x="22.86" y="-10.16" length="middle" rot="R180"/>
<pin name="PB5" x="22.86" y="-12.7" length="middle" rot="R180"/>
<pin name="PB6" x="22.86" y="-15.24" length="middle" rot="R180"/>
<pin name="PB7" x="22.86" y="-17.78" length="middle" rot="R180"/>
<pin name="BOOT0" x="22.86" y="-20.32" length="middle" rot="R180"/>
<pin name="PB8" x="22.86" y="-22.86" length="middle" rot="R180"/>
<pin name="PB9" x="22.86" y="-25.4" length="middle" rot="R180"/>
<pin name="VSS47" x="22.86" y="-27.94" length="middle" rot="R180"/>
<pin name="VDD48" x="22.86" y="-30.48" length="middle" rot="R180"/>
<text x="-17.78" y="30.48" size="1.27" layer="94">STM32F030CC</text>
<text x="-17.78" y="-35.56" size="1.27" layer="94">&gt;NAME</text>
</symbol>
</symbols>
<devicesets>
<deviceset name="STM32F030CC">
<gates>
<gate name="G$1" symbol="STM32F030CC" x="0" y="0"/>
</gates>
<devices>
<device name="" package="STM32F030CC">
<connects>
<connect gate="G$1" pin="BOOT0" pad="44"/>
<connect gate="G$1" pin="NRST" pad="7"/>
<connect gate="G$1" pin="PA0" pad="10"/>
<connect gate="G$1" pin="PA1" pad="11"/>
<connect gate="G$1" pin="PA10" pad="31"/>
<connect gate="G$1" pin="PA11" pad="32"/>
<connect gate="G$1" pin="PA12" pad="33"/>
<connect gate="G$1" pin="PA13" pad="34"/>
<connect gate="G$1" pin="PA14" pad="37"/>
<connect gate="G$1" pin="PA15" pad="38"/>
<connect gate="G$1" pin="PA2" pad="12"/>
<connect gate="G$1" pin="PA3" pad="13"/>
<connect gate="G$1" pin="PA4" pad="14"/>
<connect gate="G$1" pin="PA5" pad="15"/>
<connect gate="G$1" pin="PA6" pad="16"/>
<connect gate="G$1" pin="PA7" pad="17"/>
<connect gate="G$1" pin="PA8" pad="29"/>
<connect gate="G$1" pin="PA9" pad="30"/>
<connect gate="G$1" pin="PB0" pad="18"/>
<connect gate="G$1" pin="PB1" pad="19"/>
<connect gate="G$1" pin="PB10" pad="21"/>
<connect gate="G$1" pin="PB11" pad="22"/>
<connect gate="G$1" pin="PB12" pad="25"/>
<connect gate="G$1" pin="PB13" pad="26"/>
<connect gate="G$1" pin="PB14" pad="27"/>
<connect gate="G$1" pin="PB15" pad="28"/>
<connect gate="G$1" pin="PB2" pad="20"/>
<connect gate="G$1" pin="PB3" pad="39"/>
<connect gate="G$1" pin="PB4" pad="40"/>
<connect gate="G$1" pin="PB5" pad="41"/>
<connect gate="G$1" pin="PB6" pad="42"/>
<connect gate="G$1" pin="PB7" pad="43"/>
<connect gate="G$1" pin="PB8" pad="45"/>
<connect gate="G$1" pin="PB9" pad="46"/>
<connect gate="G$1" pin="PC13" pad="2"/>
<connect gate="G$1" pin="PC14-OSC32_IN" pad="3"/>
<connect gate="G$1" pin="PC15-OSC32_OUT" pad="4"/>
<connect gate="G$1" pin="PF0-OSC_IN" pad="5"/>
<connect gate="G$1" pin="PF1-OSC_OUT" pad="6"/>
<connect gate="G$1" pin="VDD1" pad="1"/>
<connect gate="G$1" pin="VDD24" pad="24"/>
<connect gate="G$1" pin="VDD36" pad="36"/>
<connect gate="G$1" pin="VDD48" pad="48"/>
<connect gate="G$1" pin="VDDA" pad="9"/>
<connect gate="G$1" pin="VSS23" pad="23"/>
<connect gate="G$1" pin="VSS35" pad="35"/>
<connect gate="G$1" pin="VSS47" pad="47"/>
<connect gate="G$1" pin="VSSA" pad="8"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="U1" library="lights" deviceset="STM32F030CC" device=""/>
</parts>
<sheets>
<sheet>
<plain>
</plain>
<instances>
<instance part="U1" gate="G$1" x="0" y="0"/>
</instances>
<busses>
</busses>
<nets>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
</eagle>
