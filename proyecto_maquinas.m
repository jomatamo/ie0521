



%s = 0:1/5000:100; %vector de deslizamiento
%Parametros del motor
f=60; %frecuencia
P=6;  %Polos
n_s=(60*f)/(P/2); % velocidad sincronica en RPM
n_s_pu=1; % velocidad sincronica en pu
w_s=(2*pi()*n_s)/60; %velocidad sincronica en rads/s
s=(n_s_pu-n_r_pu)/(n_s_pu); %deslizamiento
%n_r=(1-s)*n_s; %velocidad del rotor con respecto del deslizamiento en RPM
n_r_pu = 0:1/5000:1; %velocidad del roto en pu
w_r=(2*pi()*n_r_pu)/60; %velociad del rotor con respecto del dezlicamiento en rad/s

R1= 0.0453;
rs=R1;
X2= 0.0322;
xlr = X2;
R2= 0.0222;
rr= R2;
X1= 0.0775;
xls=X1;
Xm= 2.042;
xm=Xm;
H=0.5;



Zth=(i*Xm*(R1+i*X1))/(R1+i*(X1+Xm));
Rth=real(Zth);
Xth=imag(Zth);




%Modelo 1


T_ind = ((V1^2)*Xm^2*R2/s)/(



