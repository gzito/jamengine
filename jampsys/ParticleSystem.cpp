#include "stdafx.h"

#ifdef OLDPSYS
/*
Function createBubble(sprite, pivot, howmany, pSyst.PSparticleStruct)
	steps% = 5
	PSsetLifeCycle(pSyst,howmany )
	PSsetEntity	(pSyst,sprite,PS_SPRITE, 3)
	PSsetPosition	(pSyst, Rnd(-.02,.02), 0, Rnd(-.02,.02), 1, 1, 1)
	PSsetScale	(pSyst, .3, .3, 0, 1.01, 1.01, 1.01)
	PSsetSpeed	(pSyst,0,0,0,.08,0,.08)
	PSsetAlpha(pSyst,1,0)
	pEm.PSparticleEmitterStruct = New PSparticleEmitterStruct

	PSsetEmitter(pEm, howmany ,3, 1, "Big Bubble")
	PSsetPivot(pEM,pivot)

	PSsetWind(pEm,0, (Rnd(1, 5)/30)/.3, 0)
	createEmitter(pEm, pSyst);,  CreatePivot())
End Function 
*/
/*
;Include "ParticleSys_h.bb"
;Include "ParticleSys_utils.bb"

;----------------------------------------------------------------------------------------
; TO DO...
; - Specificare se segue la rotazione del pivot e le sue dimensioni?
; - Stop/freeze emitter 
;----------------------------------------------------------------------------------------


;NOTE:
; - Il sistema dell'emettitore usa coordinate globali!
; - Esiste una sola entita' di riferimento per gli spostamenti di 1 solo emettitore
;----------------------------------------------------------------------------------------

DebugLog("ParticleSys.bb included")

;*****************************************************************************
;*** Function UpdateParticles
;*** Update all the particles in the scene
;*** If the particle become "OLD" the system choose to re-emit or delete it
;*** If the emitter has no particles, then is deleted !
;*****************************************************************************
Function updateParticles( )
	;steps = 1
	;If steps<=0 Then Return
	
	;ParticleSystem\steps = ParticleSystem\steps + 1
	;If ParticleSystem\steps < steps Then
	;	Return
	;End If
	
	PSys\totParticles = 0
	PSys\activeParticles =0
	PSys\steps = 0
;____________________________________________________________________________________________________________	
.BEGIN

	For p.PSparticle = Each PSparticle
		PSys\totParticles = PSys\totParticles + 1
			
		If p\counter>0     ; *** Not yet ready / emitted ***
			p\counter = p\counter - 1 ;step				
		
			If p\counter >0 Then ;*** Consider the new counter ***
				Goto NEXT_PARTICLE ; *** Now is ready ? ***
			End If
				
				;If p\counter >0 Then Goto UPDATE ; *** Now is ready ? ***
		 		; *** Particle ready to Start ***
			;Goto NEXT_PARTICLE ; *** Now is ready ? ***				
	;____________________________________________________________________________________________________________
	.FIRST_TIME
				p\emitted = True
				;p\emitter\template.particleStruct = p\emitter\template
	;*** Set all the position of the regenerated particle ***
	;****** Initial values are stored in : p\emitter\template *******
				
				If Handle(p\emitter) = 0 Then
					PSError("The Emitter of a particle is died!")
					PSdeleteParticle(p)
					Goto NEXT_PARTICLE
				EndIf
				
				If Handle(p\emitter\pivot) 
					If p\emitter\pivot\handler = 0 
						PSError("The Emitter of a particle is died!")
						PSdeleteParticle(p)
						Goto NEXT_PARTICLE
					Else
						EntityParent p\entity, p\emitter\pivot\handler
					End If
	;**** Parental settings ****			
;				If Handle(p\emitter\pivot)	Then								
;					EntityParent p\entity, p\emitter\pivot\handler
;				End If
				
				EndIf
				
	;**** Starting Position ****
				If Handle(p\emitter\template\position\range) 				
					PositionEntity p\entity,p\x + Rnd( -p\emitter\template\position\range\x,p\emitter\template\position\range\x) ,  p\y  + Rnd(-p\emitter\template\position\range\y,p\emitter\template\position\range\y) , p\z + Rnd(-p\emitter\template\position\range\z,p\emitter\template\position\range\z)
				Else				
					PositionEntity p\entity,p\x,  p\y  , p\z
				End If
				
				EntityParent p\entity, 0				
	
	;*** Recalculate directions (depends of pivot's orientation and scale ***
				If Handle(p\emitter\template\speed)	
					If Handle(p\emitter\template\speed\direction) 
						If Handle(p\emitter\pivot)
							If Handle(p\emitter\template\speed\directionRange)					
								x# = (p\emitter\template\speed\direction\x + Rnd( -p\emitter\template\speed\directionRange\x, p\emitter\template\speed\directionRange\x))
								y# = (p\emitter\template\speed\direction\y + Rnd( -p\emitter\template\speed\directionRange\y, p\emitter\template\speed\directionRange\y))
								z# = (p\emitter\template\speed\direction\z + Rnd( -p\emitter\template\speed\directionRange\z, p\emitter\template\speed\directionRange\z))
							Else
								x# = (p\emitter\template\speed\direction\x)
								y# = (p\emitter\template\speed\direction\y)
								z# = (p\emitter\template\speed\direction\z)
							End If
							TFormVector  x / p\emitter\pivot\ScaleX, y / p\emitter\pivot\ScaleY, z  / p\emitter\pivot\ScaleZ, p\emitter\pivot\handler, 0
							p\dx=TFormedX()
							p\dy=TFormedY()
							p\dz=TFormedZ()	
						Else
							If Handle(p\emitter\template\speed\directionRange)
								p\dx = (p\emitter\template\speed\direction\x + Rnd( -p\emitter\template\speed\directionRange\x, p\emitter\template\speed\directionRange\x))
								p\dy = (p\emitter\template\speed\direction\y + Rnd( -p\emitter\template\speed\directionRange\y, p\emitter\template\speed\directionRange\y))
								p\dz = (p\emitter\template\speed\direction\z + Rnd( -p\emitter\template\speed\directionRange\z, p\emitter\template\speed\directionRange\z))
							Else
								p\dx = (p\emitter\template\speed\direction\x) 
								p\dy = (p\emitter\template\speed\direction\y) 
								p\dz = (p\emitter\template\speed\direction\z) 
							End If
						End If	
					End If
	
		;*** Recalculate acceleration (depends of pivot's orientation and scale ***	
					If Handle(p\emitter\template\speed\acceleration)			
						If Handle(p\emitter\template\speed\accelerationRange)
							p\ax = (p\emitter\template\speed\acceleration\x + Rnd( -p\emitter\template\speed\accelerationRange\x, p\emitter\template\speed\accelerationRange\x)) 
							p\ay = (p\emitter\template\speed\acceleration\y + Rnd( -p\emitter\template\speed\accelerationRange\y, p\emitter\template\speed\accelerationRange\y)) 
							p\az = (p\emitter\template\speed\acceleration\z + Rnd( -p\emitter\template\speed\accelerationRange\z, p\emitter\template\speed\accelerationRange\z)) 
						Else
							p\ax = p\emitter\template\speed\acceleration\x
							p\ay = p\emitter\template\speed\acceleration\y
							p\az = p\emitter\template\speed\acceleration\z
						End If
					End If		
				End If  ;// Handle(p\emitter\template\speed)	
	
	;*** Recalculate size  ***			
				If Handle( p\emitter\template\size)
					If Handle(p\emitter\template\size\scale)
						p\sizeX = p\emitter\template\size\scale\x	
						p\sizeY = p\emitter\template\size\scale\y
						p\sizeZ = p\emitter\template\size\scale\z
					End If	
				End If
				
	;*** Recalculate alpha  ***			
				If Handle(p\emitter\template\alpha)
					p\alpha = p\emitter\template\alpha\start
				End If
				
	;*** Recalculate colors  ***				
				If Handle( p\emitter\template\colors)
					p\colR = p\emitter\template\colors\color\x 
					p\colG = p\emitter\template\colors\color\y
					p\colB = p\emitter\template\colors\color\z
					EntityColor p\Entity,p\colR,p\colG,p\colB
				End If
	;*** Ready to be emitted now ***							
				ShowEntity p\entity
				;UpdateWorld
				;Goto NEXT_PARTICLE
		End If ;// counter > 0
		
		;*** Qui ci passa solo se deve emettere la particella ***
		;*** Controlla se deve emetterla nuovamente ***
		If p\emitter\status=PS_FREEZE_STATUS Then
			HideEntity p\entity
			Goto NEXT_PARTICLE 
		Else If p\emitter\status=PS_TERMINATE_STATUS	Then
			p\loops=1	
			p\duration = 0
			HideEntity p\entity			
			Goto NEXT_PARTICLE 														
		End If
				
		If p\duration > 0  ;	*** Never emitted before now ? ***
			p\duration = p\duration - 1 ;step						
			
			If p\duration =< 0 
				; *** RE-EMIT the particle ***	
			
				If (p\loops=0) Then Goto EMIT1 ;*** Infinite Loop ***
;*** Lifecycle controls ****				
				p\loops = p\loops-1			
				If (p\loops<=0)	; *** No more creations	needed ***				
					FreeEntity p\entity
					p\emitter\nrAlive = p\emitter\nrAlive - 1
					If (p\emitter\nrAlive=0)  ;*** Release the emitter ***
						;FreeEntity p\emitter\pivot 
						PSys\totParticleEmitters = PSys\totParticleEmitters -1	
						PSys\totParticles = PSys\totParticles - 1
						PSDeleteTemporaryPivot(p\emitter)										
						Delete p\emitter ;*** The emitter is deleted
						;PSdeleteEmitter(p\emitter)
					End If
					PSys\activeParticles = PSys\activeParticles - 1			
					Delete p ;*** The particle is deleted
					;PSDeleteParticle(p)
					Goto NEXT_PARTICLE 
				Else
					;p = Last PSparticle
					Goto NEXT_PARTICLE
				End If ;// (p\loops<=0)
;____________________________________________________________________________________________________________
.EMIT1
						
;*** Now Emit the particle ! ****
				HideEntity p\entity
				p\emitted = True
				p\duration 	 = p\emitter\template\LifeCycle\duration  			    		           
				p\x	 		 = p\emitter\template\position\start\x
				p\y	 		 = p\emitter\template\position\start\y
				p\z	 		 = p\emitter\template\position\start\z
				p\counter    = p\startCounter		
				If Handle(p\emitter\template\size) Then 
					If Handle(p\emitter\template\size\scale)				
						If p\emitter\template\entity\typeOf = PS_MESH
							p\sizeX = p\emitter\template\size\scale\x
							p\sizeY = p\emitter\template\size\scale\y
							p\sizeZ = p\emitter\template\size\scale\z				
							ScaleEntity p\entity, p\sizex,p\sizey,p\sizez
						Else If p\emitter\template\entity\typeOf = PS_SPRITE					
							p\sizeX = p\emitter\template\size\scale\x
							p\sizeY = p\emitter\template\size\scale\y
							ScaleSprite p\entity, p\sizex,p\sizey
						End If
					End If
				End If
				;MoveEntity p\entity,p\dx,p\dy,p\dz			
				Goto NEXT_PARTICLE 
			;Else
			;	Goto UPDATE 
			End If ;//  p\duration > 0
;____________________________________________________________________________________________________________
.UPDATE							
			;*** updating the size ***
			If Handle(p\emitter\template\size) ;// if #1
				If Handle(p\emitter\template\size\stepScale) 
					If p\emitter\template\entity\typeOf = PS_MESH Then
						p\sizex = p\sizex *  p\emitter\template\size\stepScale\X
						p\sizey = p\sizey *  p\emitter\template\size\stepScale\Y
						p\sizez = p\sizez *  p\emitter\template\size\stepScale\Z
						ScaleEntity p\entity, p\sizex,p\sizey,p\sizez,parent
					Else If p\emitter\template\entity\typeOf = PS_SPRITE Then
						p\sizex = p\sizex *  p\emitter\template\size\stepScale\X
						p\sizey = p\sizey *  p\emitter\template\size\stepScale\Y
						ScaleSprite p\entity, p\sizex,p\sizey
					End If
				End If
			End If
			
			;*** updating the colors ***
			If Handle(p\emitter\template\colors) ;// if #2
				If Handle(p\emitter\template\colors\endColor)
					If p\colR <> p\emitter\template\colors\endColor\x Then p\colR = p\colR + p\emitter\template\colors\stepColor\x
					If p\colG <> p\emitter\template\colors\endColor\y Then p\colG = p\colG + p\emitter\template\colors\stepColor\y 
					If p\colB <> p\emitter\template\colors\endColor\z Then p\colB = p\colB + p\emitter\template\colors\stepColor\z
				;Else
				;	p\colR = (p\colR + p\emitter\template\colors\stepColor\x) Mod 256
				;	p\colG = (p\colG + p\emitter\template\colors\stepColor\y) Mod 256
				;	p\colB = (p\colB + p\emitter\template\colors\stepColor\z) Mod 256
				End If
				EntityColor p\Entity,p\colR,p\colG,p\colB
			End If
			
			;*** updating the alpha ***
			If Handle(p\emitter\template\Alpha) ;// if #3
				
				;DebugLog(p\alpha + " , " +p\emitter\template\alpha\stepa)
				p\alpha = p\alpha + p\emitter\template\alpha\stepa
				If p\alpha<0 Then 
					p\alpha = 0
					;*** delete me! I'm invisible
					p\duration = 0
					HideEntity p\entity
				Else If p\alpha>1 Then
					p\alpha = 1
				End If
				EntityAlpha p\entity, p\alpha
			End If
			
			;*** Updating particle positions ***
			If Handle(p\emitter\template\speed) ;// if #4		
				;If Handle(p\emitter\template\speed\direction)
					p\ox = p\x : p\oy = p\y : p\oz = p\z	
					If Handle(p\emitter\template\speed\acceleration) 		
						p\dx = p\dx * p\ax
						p\dy = p\dy * p\ay
						p\dz = p\dz * p\az
					End If
				;End If
			End If	
			
			If p\emitter\template\RotationAngle
				p\RotationAngle = p\RotationAngle + p\RotationAngleSpeed
				If p\emitter\template\entity\typeOf = PS_SPRITE Then
					RotateSprite p\Entity,p\RotationAngle
				End If
			End If
;*** Move the entity/particle on its directions ***
			MoveEntity p\entity,p\dx ,p\dy, p\dz
;*** Applying the wind ( Translate the particle ) ***
			If Handle(p\emitter\wind) ;// if #5	
				TranslateEntity p\entity,p\emitter\wind\x , p\emitter\wind\y , p\emitter\wind\z,False 
			End If 	
			PSys\activeParticles = PSys\activeParticles + 1 
			
		End If ;//  p\duration > 0
;____________________________________________________________________________________________________________		
.NEXT_PARTICLE
	Next
;*** Recalculate Maxs ***
	If PSys\maxShowedParticles < PSys\activeParticles Then PSys\maxShowedParticles = PSys\activeParticles
	If PSys\maxShowedEmitters < PSys\totParticleEmitters Then PSys\maxShowedEmitters = PSys\totParticleEmitters
;____________________________________________________________________________________________________________	
End Function



;*****************************************************************************
;*** Function CreateEmitter
;*** Create an emitter based on the particle struct and emitter struct
;*** passed as parameter. Note that there is only 1 pivot and 1 entity for
;*** the particles
;*****************************************************************************

Function CreateEmitter.PSparticleEmitter( tempEmitter.PSparticleEmitterStruct , tempPart.PSparticleStruct )
;*** NOTE: all particles of an emitter use the same starting tempPart entity!
;*** 1 emitter 1 entity

;**** The Emitter ****
	pEmitter.PSparticleEmitter = New PSparticleEmitter							; create the new emitter

	pEmitter\wind.vector = tempEmitter\wind									; setting global wind
	pEmitter\nrAlive%=tempEmitter\howmany										; how many particles
	;pEmitter\status= tempEmitter\status											; status (*)
	pEmitter\template = tempPart 													; create the particle model
	pEmitter\name = tempEmitter\name
	
	Local startCounter%=1
	If Handle(tempEmitter\pivot)=0
;		pEmitter\pivot = New PSpivot
;		pEmitter\pivot\behavior = PS_TEMPORARY_PIVOT
;		pEmitter\pivot\handler = CreatePivot()
;		pEmitter\pivot\ScaleX = 1 
;		pEmitter\pivot\ScaleY = 1
;		pEmitter\pivot\ScaleZ = 1 
	Else
		pEmitter\pivot = New PSpivot
		pEmitter\pivot\handler = (tempEmitter\pivot\handler)
		;HideEntity pEmitter\pivot\handler		
		pEmitter\pivot\ScaleX = tempEmitter\pivot\ScaleX 
		pEmitter\pivot\ScaleY = tempEmitter\pivot\ScaleY
		pEmitter\pivot\ScaleZ = tempEmitter\pivot\ScaleZ 
		pEmitter\pivot\behavior = tempEmitter\pivot\behavior  
	End If
;____________________________________________________________________________________________________________	
;**** Entity operations ****
	HideEntity tempPart\entity\handler
	EntityBlend tempPart\entity\handler, tempPart\entity\blendMode							; set the entity blend

;*** Scaling the entity to the right size ***
	If Handle(tempPart\size)
		If tempPart\entity\typeOf = PS_MESH Then
			ScaleEntity tempPart\entity\handler ,tempPart\size\scale\X, tempPart\size\scale\Y,tempPart\size\scale\Z
		Else If tempPart\entity\typeOf = PS_SPRITE Then
			ScaleSprite tempPart\entity\handler ,tempPart\size\scale\X, tempPart\size\scale\Y
		Else
			PSError("Unknown Entity TypeOf" )
			;*** Error : no type defined
		End If
		;tempPart\dSize\x = (tempPart\fsize\x - tempPart\size\x) / tempPart\duration
		;tempPart\dSize\y = (tempPart\fsize\y - tempPart\size\y) / tempPart\duration
		;tempPart\dSize\z = (tempPart\fsize\z - tempPart\size\z) / tempPart\duration
	End If
	
;*** Setting the alpha ***
	If Handle(tempPart\Alpha)	
		tempPart\alpha\stepa = (tempPart\alpha\end - tempPart\alpha\start) / tempPart\lifecycle\duration
		EntityAlpha tempPart\entity\handler,tempPart\alpha\start
		If tempPart\alpha\fadeNext#<>tempPart\alpha\fadeFar#
			EntityAutoFade tempPart\entity\handler,tempPart\alpha\fadeNext,tempPart\alpha\fadeFar
		End If
	End If

;*** Setting colors cycle ***
	If Handle(tempPart\colors) 	
		If Handle(tempPart\colors\endcolor) 
			tempPart\colors\stepColor = New vector
			tempPart\colors\stepColor\x = (tempPart\colors\endcolor\x - tempPart\colors\color\x) / tempPart\lifecycle\duration
			tempPart\colors\stepColor\y = (tempPart\colors\endcolor\y - tempPart\colors\color\y) / tempPart\lifecycle\duration
			tempPart\colors\stepColor\z = (tempPart\colors\endcolor\z - tempPart\colors\color\z) / tempPart\lifecycle\duration
		End If
		EntityColor tempPart\entity\handler, tempPart\colors\Color\x, tempPart\colors\Color\y , tempPart\colors\Color\z
	End If

;**** Starting Position	needed ***
	If Handle(tempPart\position)=0 Then
		tempPart\position = New PSPosition
		PSError("Particle start Position not defined. Using default value (0,0,0)")
	End If
	
	
;*** Create the particles collection ***
;____________________________________________________________________________________________________________	
	For t=1 To tempEmitter\howMany 
		For g = 1 To tempEmitter\groups
			part.PSparticle = New PSParticle
			
;**** Duration & Lifecycle ****					
			part\duration 				= tempPart\lifecycle\duration +  Rand(-tempPart\lifecycle\durationRange, tempPart\lifecycle\durationRange)
			part\startCounter			= startCounter
			part\counter 				= startCounter	
			part\emitter 				= pEmitter	; << Reverse Control (particle knows its emitter!)	
			part\loops 					= tempPart\lifeCycle\loops
			part\emitted = False
				
;**** Position ****			
			If Handle(tempPart\position\start) Then                			
				part\x	 					= tempPart\position\start\x
				part\y	 					= tempPart\position\start\y
				part\z	 					= tempPart\position\start\z	
	      End If            							
			part\ox = part\x : part\oy	= part\y : part\oz = part\z
			
;**** Direction ****
			If Handle(tempPart\speed) Then
				If Handle(tempPart\speed\direction) Then   	            			
					part\dx	 					= tempPart\speed\direction\x
					part\dy	 					= tempPart\speed\direction\y
					part\dz						= tempPart\speed\direction\z
				End If
			End If	
;**** Size & Scale ****
			If Handle(tempPart\size)
				part\sizeX = tempPart\size\scale\x
				part\sizeY = tempPart\size\scale\y
				part\sizeZ = tempPart\size\scale\z
			End If
			If tempPart\RotationAngle#
				part\RotationAngle = tempPart\RotationAngle
				part\RotationAngleSpeed = tempPart\RotationAngleSpeed
			End If
;**** Colors ****	
			If Handle(tempPart\Colors)	Then
				part\colR%      = tempPart\colors\color\x                   
				part\colG%      = tempPart\colors\color\y                   
				part\colB%		= tempPart\colors\color\z
			End If
;**** Alpha Transparency ****			
			If Handle(tempPart\alpha)
				part\alpha = tempPart\alpha\start
			End If
			
;**** Entities ****			
			part\entity = CopyEntity(tempPart\entity\handler) ; <<<< This takes a lot of time!
			HideEntity part\entity
		Next ;g
		t = t + tempEmitter\groups - 1		
		startCounter = startCounter + tempEmitter\distance
	Next ;t
;____________________________________________________________________________________________________________	
;**** Last operarations ****
	PSys\totParticleEmitters = PSys\totParticleEmitters + 1    ; new emitter in the world
	Return pEmitter
End Function
*/
#endif
