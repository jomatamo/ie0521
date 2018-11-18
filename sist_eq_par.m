function dydt = odefun(t,y,wb,xm,xrr,D,vqs,vdr,vqr,vds,rr,rs,H,Tlmec)
dydt = zeros(5,1);

dydt(1) = wb*(vqs-(rs*xrr/D)*y(1)-(ws/wb)*y(2)+((rs*xm)/D)*y(3));
dydt(2) = wb*(vds-(rs*xrr/D)*y(2)-(ws/wb)*y(1)+((rs*xm)/D)*y(4));
dydt(3) = wb*(vqr-(rr*xm/D)*y(1)-(rr*xss/D)*y(3)+(ws/wb)*y(4)+(y(5)/wb)*y(4));
dydt(4) = wb*(vdr-(rr*xm/D)*y(2)-(rr*xss/D)*y(4)+(ws/wb)*y(3)+(y(5)/wb)*y(3));
dydt(5) = (wb/2*H)*((xm/D)*(y(1)*y(4)-y(3)*y(2))-Tlmec);

end
