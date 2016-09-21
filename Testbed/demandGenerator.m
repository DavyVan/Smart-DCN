% %% ---------------average -----------------------
% 
% level  = 3;
% demandSize = 8;
% demandRange =250;
% demandMAX  = demandRange * level;
% sample_num = 20000;
% 
% for ii= 1:sample_num
%     fprintf('%d the sample writin... \n',ii);
%     demand = zeros(demandSize,demandSize);
%     cnt = 28*2;
%     while cnt > 0
%         pos = unidrnd(demandSize,1,2);
%         if pos(1) ~= pos(2) && demand(pos(1),pos(2)) == 0
%             demand(pos(1),pos(2)) = unidrnd(demandMAX);
%             cnt = cnt-1;
%         end
%     end
%     fid = fopen(['C:\Users\wmw\Desktop\source code\NSDI\Sample_data4\hotspot\avg3-0\demand\' 'demand' num2str(ii-1) '.txt'],'wt');
%     for i=1:demandSize
%         for j=1:demandSize
%             p = demand(i,j);
%             fprintf(fid,'%d ',p);
%         end
%         fprintf(fid,'%c\n','');
%     end
%     fclose(fid);
% end
% 

%% ---------------hotspot ---------------------

level  = 1;
hotspot_per = 0.7;

hotspotLevel =2;
demandSize = 10000;
demandRange =500;
demandMAX  = demandRange * level;
sample_num = 1;

demand_num = demandSize^2-demandSize;

hotspot_num = demand_num  * hotspot_per;  % demand(i,j) <-> demand(j,i)

for ii= 1:sample_num
    fprintf('%d the sample writing... \n',ii);
    demand = zeros(demandSize,demandSize);
%     cnt_avg = demand_num  - hotspot_num;
    cnt_hotspot = hotspot_num;
    while floor(cnt_hotspot)> 0
        pos = unidrnd(demandSize,1,2);
        if pos(1) ~= pos(2) && demand(pos(1),pos(2)) == 0
            d_temp = unidrnd(demandMAX,1,2);
            demand(pos(1),pos(2)) = d_temp(1)+demandMAX*(hotspotLevel-1);
            demand(pos(2),pos(1)) = d_temp(2)+demandMAX*(hotspotLevel-1);
            cnt_hotspot = cnt_hotspot-2;
        end
    end

    
%     for i = 1:demandSize
%         for j =1:demandSize
%             if i ~= j && demand(i,j)==0
%                 demand(i,j) = unidrnd(demandMAX);
%                 cnt_avg = cnt_avg-1;
%             end
%         end
%     end
    
        fid = fopen('demand0.txt','wt');
        for i=1:demandSize
            for j=1:demandSize
                p = demand(i,j);
                fprintf(fid,'%d ',p);
            end
            fprintf(fid,'%c\n','');
        end
        fclose(fid);
        
        fid = fopen('Edmond-testbed\Edmond-testbed\Sample_data\edmond_4\demand0\demand0.txt','wt');
        for i=1:demandSize
            for j=1:demandSize
                p = demand(i,j);
                fprintf(fid,'%d ',p);
            end
            fprintf(fid,'%c\n','');
        end
        fclose(fid);
        
%         fid = fopen('Edmond-testbed\Edmond-testbed\Sample_data\edmond_4\demand1\demand0.txt','wt');
%         for i=1:demandSize
%             for j=1:demandSize
%                 p = demand(i,j);
%                 fprintf(fid,'%d ',p);
%             end
%             fprintf(fid,'%c\n','');
%         end
%         fclose(fid);
%         
%         fid = fopen('Edmond-testbed\Edmond-testbed\Sample_data\edmond_4\demand2\demand0.txt','wt');
%         for i=1:demandSize
%             for j=1:demandSize
%                 p = demand(i,j);
%                 fprintf(fid,'%d ',p);
%             end
%             fprintf(fid,'%c\n','');
%         end
%         fclose(fid);
%         
%         fid = fopen('Edmond-testbed\Edmond-testbed\Sample_data\edmond_4\demand3\demand0.txt','wt');
%         for i=1:demandSize
%             for j=1:demandSize
%                 p = demand(i,j);
%                 fprintf(fid,'%d ',p);
%             end
%             fprintf(fid,'%c\n','');
%         end
%         fclose(fid);
%         
%         fid = fopen('Edmond-testbed\Edmond-testbed\Sample_data\edmond_4\demand4\demand0.txt','wt');
%         for i=1:demandSize
%             for j=1:demandSize
%                 p = demand(i,j);
%                 fprintf(fid,'%d ',p);
%             end
%             fprintf(fid,'%c\n','');
%         end
%         fclose(fid);
        
%         fid = fopen('Edmond-testbed\Edmond-testbed\deeplearning-testbed\Sample_Data\Demand\demand0.txt','wt');
%         for i=1:demandSize
%             for j=1:demandSize
%                 p = demand(i,j);
%                 fprintf(fid,'%d ',p);
%             end
%             fprintf(fid,'%c\n','');
%         end
%         fclose(fid);
%         
%         fid = fopen('deeplearning-testbed\deeplearning-testbed\Sample_Data\Demand\demand0.txt','wt');
%         for i=1:demandSize
%             for j=1:demandSize
%                 p = demand(i,j);
%                 fprintf(fid,'%d ',p);
%             end
%             fprintf(fid,'%c\n','');
%         end
%         fclose(fid);
end