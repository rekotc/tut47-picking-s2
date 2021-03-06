////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "applicationclass.h"


ApplicationClass::ApplicationClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_TextureShader = 0;
	m_LightShader = 0;
	m_Light = 0;
	//m_Text = 0;
	m_Bitmap = 0;
	//bCube = 0;
	//bCube2 = 0;
	pickedUpColors[0] = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	pickedUpColors[1] = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	pickedUpMesh = 0;
}


ApplicationClass::ApplicationClass(const ApplicationClass& other)
{
}


ApplicationClass::~ApplicationClass()
{
}


bool ApplicationClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	bool result;
	XMMATRIX baseViewMatrix;
	
	// Store the screen width and height.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(10.0f, 10.0f, -5.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	// Create the model object.
	m_Model = new ModelClass;
	if(!m_Model)
	{
		return false;
	}
	//m_Model->setId(0);
	// Initialize the model object.
	result = m_Model->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), "data/dice.txt", "data/dice.tga");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if(!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if(!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if(!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);

	/*// Create the text object.
	m_Text = new TextClass;
	if(!m_Text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}
	*/
	// Create the bitmap object.
	m_Bitmap = new BitmapClass;
	if(!m_Bitmap)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_Bitmap->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), screenWidth, screenHeight, "data/mouse.tga", 32, 32);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	// Initialize that the user has not clicked on the screen to try an intersection test yet.
	m_beginCheck = false;

	//bCube = new AabbClass();
	//bCube2 = new AabbClass();
	pickedUpMesh = new CollisionClass();

	return true;
}


void ApplicationClass::Shutdown()
{
	// Release the bitmap object.
	if(m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}

/*	// Release the text object.
	if(m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}*/

	// Release the light object.
	if(m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if(m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the texture shader object.
	if(m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the model object.
	if(m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}
	
	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	
	
	return;
}


bool ApplicationClass::Frame(int mouseX, int mouseY)
{
	bool result;

	m_mouseX = mouseX;
	m_mouseY = mouseY;

	// Handle the input processing.
	result = HandleInput();
	if(!result)
	{
		return false;
	}

	// Render the graphics scene.
	result = Render();
	if(!result)
	{
		return false;
	}

	return true;
}


bool ApplicationClass::HandleInput()
{
	bool result;
	int mouseX, mouseY;


	/*// Do the input frame processing.
	result = m_Input->Frame();
	if(!result)
	{
		return false;
	}

	// Check if the user pressed escape and wants to exit the application.
	if(m_Input->IsEscapePressed() == true)
	{
		return false;
	}

	// Check if the left mouse button has been pressed.
	if(m_Input->IsLeftMouseButtonDown() == true)
	{
		// If they have clicked on the screen with the mouse then perform an intersection test.
		if(m_beginCheck == false)
		{
			m_beginCheck = true;
			m_Input->GetMouseLocation(mouseX, mouseY);
			TestIntersection(mouseX, mouseY);
		}
	}

	// Check if the left mouse button has been released.
	if(m_Input->IsLeftMouseButtonDown() == false)
	{
		m_beginCheck = false;
	}
	*/
	return true;
}


bool ApplicationClass::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix, translateMatrix;
	bool result;
		

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
	
	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);
	
	
	// Translate to the location of the sphere.
	translateMatrix = XMMatrixTranslation(10.0f, 10.0f, 10.0f);
	worldMatrix		= XMMatrixMultiply(worldMatrix, translateMatrix);

	// Render the model using the light shader.
	m_Model->Render(m_D3D->GetDeviceContext());
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture(), m_Light->GetDirection(),pickedUpColors[0]);
	if(!result)
	{
		return false;
	}
	
	// Reset the world matrix.
	m_D3D->GetWorldMatrix(worldMatrix);	

	// Translate to the location of the sphere.
	translateMatrix = XMMatrixTranslation(12.0f, 12.0f, 10.0f);
	worldMatrix = XMMatrixMultiply(worldMatrix, translateMatrix);

	
	// Render the SECOND model using the light shader.
	m_Model->Render(m_D3D->GetDeviceContext());
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture(), m_Light->GetDirection(), pickedUpColors[1]);
	if (!result)
	{
		return false;
	}
	
	// Reset the world matrix.
	m_D3D->GetWorldMatrix(worldMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

	// Turn on alpha blending.
	m_D3D->EnableAlphaBlending();

	// Get the location of the mouse from the input object,
	//m_Input->GetMouseLocation(mouseX, mouseY);

	//mouseX = 350;
	//mouseY = 250;
	// Render the mouse cursor with the texture shader.
	result = m_Bitmap->Render(m_D3D->GetDeviceContext(), m_mouseX, m_mouseY);  if(!result) { return false; }
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Bitmap->GetTexture());

	// Render the text strings.
	//result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if(!result)
	{
		return false;
	}

	// Turn of alpha blending.
	m_D3D->DisableAlphaBlending();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}


void ApplicationClass::TestIntersection(int mouseX, int mouseY)
{
	bool intersect, result;
	/*
	float pointX, pointY;
	XMMATRIX projectionMatrix, viewMatrix, inverseViewMatrix, worldMatrix, translateMatrix, inverseWorldMatrix;
	XMFLOAT3 direction, origin, rayOrigin, rayDirection;
	
	

	// Move the mouse cursor coordinates into the -1 to +1 range.
	pointX = ((2.0f * (float)mouseX) / (float)m_screenWidth) - 1.0f;
	pointY = (((2.0f * (float)mouseY) / (float)m_screenHeight) - 1.0f) * -1.0f;
		
	// Adjust the points using the projection matrix to account for the aspect ratio of the viewport.
	m_D3D->GetProjectionMatrix(projectionMatrix);
	pointX = pointX / projectionMatrix._11;
	pointY = pointY / projectionMatrix._22;

	// Get the inverse of the view matrix.
	m_Camera->GetViewMatrix(viewMatrix);
	XMMatrixInverse(&inverseViewMatrix, NULL, &viewMatrix);

	// Calculate the direction of the picking ray in view space.
	direction.x = (pointX * inverseViewMatrix._11) + (pointY * inverseViewMatrix._21) + inverseViewMatrix._31;
	direction.y = (pointX * inverseViewMatrix._12) + (pointY * inverseViewMatrix._22) + inverseViewMatrix._32;
	direction.z = (pointX * inverseViewMatrix._13) + (pointY * inverseViewMatrix._23) + inverseViewMatrix._33;

	// Get the origin of the picking ray which is the position of the camera.
	origin = m_Camera->GetPosition();

	// Get the world matrix and translate to the location of the sphere.
	m_D3D->GetWorldMatrix(worldMatrix);
	XMMATRIXTranslation(&translateMatrix, 10.0f, 10.0f, 10.0f);
	XMMATRIXMultiply(&worldMatrix, &worldMatrix, &translateMatrix); 

	// Now get the inverse of the translated world matrix.
	XMMATRIXInverse(&inverseWorldMatrix, NULL, &worldMatrix);

	// Now transform the ray origin and the ray direction from view space to world space.
	D3DXVec3TransformCoord(&rayOrigin, &origin, &inverseWorldMatrix);
	D3DXVec3TransformNormal(&rayDirection, &direction, &inverseWorldMatrix);

	// Normalize the ray direction.
	D3DXVec3Normalize(&rayDirection, &rayDirection);

	//salvo il currentId come lastId
	pickedUpMesh->setLastId(pickedUpMesh->getCurrentId());

	//resetto la pickedUpMesh
	pickedUpMesh->setCurrentId(-1);
	pickedUpMesh->setCurrentMinDistance(999999999);

	// Now perform the ray-sphere intersection test.
	//intersect = RaySphereIntersect(rayOrigin, rayDirection, 1.0f);
	intersect = RayAABBIntersect(pickedUpMesh, m_Model->getId(), rayOrigin, rayDirection, m_Model->getBoundingBox());
	
	
	//seconda sfera
	
	// Get the world matrix and translate to the location of the sphere.
	//m_D3D->GetWorldMatrix(worldMatrix);
	XMMATRIXTranslation(&translateMatrix, 1.0f, 0.0f, 5.0f);
	XMMATRIXMultiply(&worldMatrix, &worldMatrix, &translateMatrix);

	// Now get the inverse of the translated world matrix.
	XMMATRIXInverse(&inverseWorldMatrix, NULL, &worldMatrix);

	// Now transform the ray origin and the ray direction from view space to world space.
	D3DXVec3TransformCoord(&rayOrigin, &origin, &inverseWorldMatrix);
	D3DXVec3TransformNormal(&rayDirection, &direction, &inverseWorldMatrix);

	// Normalize the ray direction.
	D3DXVec3Normalize(&rayDirection, &rayDirection);

	intersect = RayAABBIntersect(pickedUpMesh, m_Model2->getId(), rayOrigin, rayDirection, m_Model2->getBoundingBox());

	*/
	//pickedUpMesh->
	//pickedUpMesh->setCurrentId(0);
	//result = setPickedUpMesh(pickedUpMesh);

	return;
}

//DA AGGIORNARE!
bool ApplicationClass::RaySphereIntersect(XMFLOAT3 rayOrigin, XMFLOAT3 rayDirection, float radius)
{
	float a, b, c, discriminant;


	// Calculate the a, b, and c coefficients.
	a = (rayDirection.x * rayDirection.x) + (rayDirection.y * rayDirection.y) + (rayDirection.z * rayDirection.z);
	b = ((rayDirection.x * rayOrigin.x) + (rayDirection.y * rayOrigin.y) + (rayDirection.z * rayOrigin.z)) * 2.0f;
	c = ((rayOrigin.x * rayOrigin.x) + (rayOrigin.y * rayOrigin.y) + (rayOrigin.z * rayOrigin.z)) - (radius * radius);

	// Find the discriminant.
	discriminant = (b * b) - (4 * a * c);

	// if discriminant is negative the picking ray missed the sphere, otherwise it intersected the sphere.
	if (discriminant < 0.0f)
	{
		return false;
	}

	return true;
}



bool ApplicationClass::RayAABBIntersect(CollisionClass* pickedUpMesh, int currentid, XMFLOAT3 rayOrigin, XMFLOAT3 rayDirection, AabbClass* bCube)
{

	

	//rayOrigin = XMFLOAT3(10.0f, 10.0f, -5.0f);
	double tmin = -INFINITY, tmax = INFINITY;

	if (rayDirection.x != 0.0) {
		double tx1 = (bCube->min[0] - rayOrigin.x) / rayDirection.x;
		double tx2 = (bCube->max[0] - rayOrigin.x) / rayDirection.x;

		tmin = max(tmin, min(tx1, tx2));
		tmax = min(tmax, max(tx1, tx2));
	}

	if (rayDirection.y != 0.0) {
		double ty1 = (bCube->min[1] - rayOrigin.y) / rayDirection.y;
		double ty2 = (bCube->max[1] - rayOrigin.y) / rayDirection.y;

		tmin = max(tmin, min(ty1, ty2));
		tmax = min(tmax, max(ty1, ty2));
	}

	if (rayDirection.z != 0.0) {
		double tz1 = (bCube->min[2] - rayOrigin.z) / rayDirection.z;
		double tz2 = (bCube->max[2] - rayOrigin.z) / rayDirection.z;

		tmin = max(tmin, min(tz1, tz2));
		tmax = min(tmax, max(tz1, tz2));
	}
	
	//se c'� l'intersezione, e il nuovo oggetto si trova ad una distanza inferiore rispetto a quello correntemente selezionato
	//aggiorna pickedUpMesh con le informazioni sul nuovo oggetto
	if (	(tmax >= tmin) && (pickedUpMesh->getCurrentMinDistance()>tmin)	){

		pickedUpMesh->setCurrentId(currentid);
		pickedUpMesh->setCurrentMinDistance(tmin);
		return  true;

	}
	else
	return false;


	/*
	float tmin = 0.0f; // set to -FLT_MAX to get first hit on line
	float tmax = 10.0f; // set to max distance ray can travel (for segment)


	float p[3];
	p[0] = rayOrigin.x;
	p[1] = rayOrigin.y;
	p[2] = rayOrigin.z;

	float d[3];
	d[0] = rayDirection.x;
	d[1] = rayDirection.y;
	d[2] = rayDirection.z;



		// For all three slabs
		for (int i = 0; i < 3; i++) {
	
				// Compute intersection t value of ray with near and far plane of slab
				float ood = 1.0f / d[i];
				float t1 = (bCube->min[i] - p[i]) * ood;
				float t2 = (bCube->max[i] - p[i]) * ood;
				// Make t1 be intersection with near plane, t2 with far plane
				if (t1 > t2){
					//Swap(t1, t2);
					float temp = t1;
					t1 = t2;
					t2 = temp;
				}
				// Compute the intersection of slab intersection intervals
				if (t1 > tmin) tmin = t1;
				if (t2 > tmax) tmax = t2;
				// Exit with no collision as soon as slab intersection becomes empty
				if (tmin > tmax) return 0;
			}
		//}
	// Ray intersects all 3 slabs. Return point (q) and intersection t value (tmin)
	//q = p + d* tmin;
	return 1;*/
}

bool ApplicationClass::setPickedUpMesh(CollisionClass* pickedUpMesh){

	bool result;

	switch (pickedUpMesh->getCurrentId()){

	case -1:
		//result = m_Text->SetIntersection(false, pickedUpMesh->getCurrentId(), m_D3D->GetDeviceContext());
		result = setPickedUpColor(-1);
		break;

	default:
		//result = m_Text->SetIntersection(true, pickedUpMesh->getCurrentId(), m_D3D->GetDeviceContext());
		result = setPickedUpColor(pickedUpMesh->getCurrentId());

	}

	return result;
}

bool ApplicationClass::setPickedUpColor(int id){

	/*
	for (int i = 0; i < sizeof(pickedUpColors) / sizeof(XMFLOAT4); i++){

	if (i == id){
		pickedUpColors[i] = XMFLOAT4(0.5f, 0.6f, 0.34f, 1.0f);
	}
	else pickedUpColors[i] = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		
	}*/
	//prendo l'id dell'ultima mesh selezionata
	int last	= pickedUpMesh->getLastId();
	int current = pickedUpMesh->getCurrentId();
	//se � diverso da -1 significa che al passo precedente avevo selezionato
	//una mesh, quindi ne resetto il colore
	if(last!=-1)pickedUpColors[last] = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	//vado poi a modificare il colore della mesh correntemente selezionata (se diversa da -1)
	if (current!=-1)pickedUpColors[current] = XMFLOAT4(0.5f, 0.6f, 0.34f, 1.0f);

	return true;

}