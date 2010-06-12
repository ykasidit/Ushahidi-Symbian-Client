Ushahidi Symbian Uploader
========================
This is an Ushahidi Symbian Client software for automatic uploading.

Purpose
========
Report incidents to an Ushahidi website automatically after the user records a video or takes a photo on the phone. This is a software applicaiton for S60 3rd and 5th (Symbian^1) edition phones.

This software might also bundle the ushahidi j2me client as a “front-end” for its “view incidents” and other features - in order to avoid redundant work on the same features again, since symbian phones already support j2me very well. (to be discussed later)

Usage example
=============
1. User starts ushahidi symbian app - if gps is supported, the user might wait for app to show that the GPS is ready.

2. User takes a picture or video normally (by pressing the phone's physical camera button, while the ushahidi symbian app is in background.)

3. When each new photo or video recording is done, the ushahidi symbian app detects that and pops-up to ask “Report incident with this photo/video?” - if “yes” - then ask some required info and then the app uploads/posts it to the configured or default ushahidi website for that build.

Note: The application would maintain a list of “incidents to report” and upload them when possible - so the incident would not be lost if it was taken in situations where the mobile network is down or out of coverage/service.

Technical details
=================
- Stamps the video/picture location via an internal GPS (if phone has one) or external bluetooth GPS (if phone doesn't have internal GPS).

- Supports S60 2nd, 3rd and 5th (Symbian^1) edition phones. See <http://www.symbian.org/devices>.
To be coded in native Symbian C++ (on Ubuntu GNU/Linux, according to the article develop symbian c++ on linux)
The source code would be deployed under the GNU GPL on Github.

Status
======
At the moment, this is the initial stage, nothing works yet - no release builds yet. Added bluetooth gps code, would test them soon and things should start to work within a month or two.


License
=======
"Ushahidi Symbian Uploader" is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

How to Compile
==============
General Symbian MMP build. Please refer to Symbian C++ (S60 3rd edition onwards) resources (forum.nokia.com, www.symbian.org) and books.

You'd also need two static lib projects - AzqLocmon (see <http://github.com/ykasidit/AzqLocmon>) and AzqGenUtils (see <http://github.com/ykasidit/AzqGenUtils>).

For freedom, as in the GNU "FREE SOFTWARE" idealogy - please see <http://www.gnu.org/philosophy/free-sw.html>, the author develops and compiles this project on Ubuntu GNU/Linux - please see instructions at <http://www.clearevo.com/blog/programming/2009/06/05/symbian_c%2B%2B_on_linux_using_gnupoc_and_eclipse_cdt.html>.

