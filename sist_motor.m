function dydt = sist_motor(t,y,w_b,w_s,xm,xss,xrr,D,rr,rs,H,Tlmec)
dydt = zeros(5,1);

dydt(1) = w_b*(1-(rs*xrr/D)*y(1)-(w_s/w_b)*y(2)+((rs*xm)/D)*y(3));
dydt(2) = w_b*(-(rs*xrr/D)*y(2)+(w_s/w_b)*y(1)+((rs*xm)/D)*y(4));
dydt(3) = w_b*((rr*xm/D)*y(1)-(rr*xss/D)*y(3)-(w_s/w_b)*y(4)+(y(5)/w_b)*y(4));
dydt(4) = w_b*((rr*xm/D)*y(2)-(rr*xss/D)*y(4)+(w_s/w_b)*y(3)-(y(5)/w_b)*y(3));
dydt(5) = (w_b/(2*H))*((xm/D)*(y(1)*y(4)-y(3)*y(2))-Tlmec);

end
