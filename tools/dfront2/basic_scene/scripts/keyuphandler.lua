-- print( "key up : " .. keyboard:GetLastKeyupCode() )

if ds:IsCurrentCamera( "scene0", cam0 ) > 0 then
 
  if keyboard:GetLastKeyupCode() == 81 then
    fps0:SetSpeed( 0.0 )
  elseif keyboard:GetLastKeydownCode() == 87 then
    fps0:SetSpeed( 0.0 )    
  end
end