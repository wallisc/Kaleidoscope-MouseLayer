#persistent
coordmode, mouse, screen

mousegetpos, lastMouseX, lastMouseY

settimer, update, 250
return

update:
mousegetpos, currentMouseX, currentMouseY
if (currentMouseX != lastMouseX or currentMouseY != lastMouseY) {
	SetNumLockState On
	mousegetpos, lastMouseX, lastMouseY  
}
else {
	SetNumLockState Off
}
return