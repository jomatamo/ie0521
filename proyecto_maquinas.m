



%s = 0:1/5000:100; %vector de deslizamiento
%Parametros del motor
f=60; %frecuencia
P=6;  %Polos
n_s=(60*f)/(P/2); % velocidad sincronica en RPM
n_s_pu=1; % velocidad sincronica en pu
w_s=(2*pi()*n_s)/60; %velocidad sincronica en rads/s
n_r_pu = 0:1/5000:1; %velocidad del roto en pu

%s=(n_s_pu-n_r_pu)/(n_s_pu); %deslizamiento
w_r = 0:1/5000:1;
s=(1-w_r);

%n_r=(1-s)*n_s; %velocidad del rotor con respecto del deslizamiento en RPM

%w_r=(2*pi()*n_r_pu)/60; %velociad del rotor con respecto del dezlicamiento en rad/s
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
V1=1;


Zth=(i*Xm*(R1+i*X1))/(R1+i*(X1+Xm));
Rth=real(Zth);
Xth=imag(Zth);
H=0.5;



%Modelo 1


T_ind = ((V1^2)*Xm^2*R2./s)./(((Rth+R2./s).^2+(Xth+X2)^2)*(R1^2+(X1+Xm)^2));

T_ind_10 = ((V1^2)*Xm^2*R2*(1.1)./s)./(((Rth+R2*(1.1)./s).^2+(Xth+X2)^2)*(R1^2+(X1+Xm)^2));

T_ind_20 = ((V1^2)*Xm^2*R2*(1.2)./s)./(((Rth+R2*(1.2)./s).^2+(Xth+X2)^2)*(R1^2+(X1+Xm)^2));


T_ind_30 = ((V1^2)*Xm^2*R2*(1.3)./s)./(((Rth+R2*(1.3)./s).^2+(Xth+X2)^2)*(R1^2+(X1+Xm)^2));

figure
plot(w_r,T_ind)
figure
plot(w_r,T_ind,w_r,T_ind_10,w_r,T_ind_20,w_r,T_ind_30)


%Modelo 2

D = xss*xrr-xm^2;
xrr = xlr+xm;
xss = xls+xm;

Tlmec=0.2;
tspn = [0 0.8]
y0 = [0 0 0 0 0];
[t,y]=ode45(@(t,y) odefun(t,y,wb,xm,xrr,D,vqs,vdr,vqr,vds,rr,rs,H,Tlmec), tspn,y0);

w_r_2=y(5);
T_ind_2= (xm/D)*(y(1).*y(4)-y(3).*y(2);




%T_ind = (xm/D)*(qs*dr-qr*ds);
