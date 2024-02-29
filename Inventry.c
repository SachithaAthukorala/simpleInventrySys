#include<stdio.h>
#include<stdlib.h>

struct inventry{
    int itemNumber;
    char itemName[20];
    int itemQty;
    double itemPrice;
};

void clear_console() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int goBackToMenu() {
    char yesNo;
    printf("\n\nWant to Go Back to Menu ? (y/n) : ");
    scanf(" %c",&yesNo);
    if(yesNo =='N' || yesNo=='n'){
        goBackToMenu();
    }else{
        return 1;
    }
}

int getLastItemNumber() {
    FILE *filePtr;
    struct inventry inv;
    int result = 0;

    filePtr = fopen("inventry.txt", "rb");
    if (filePtr == NULL) {
        printf("\nCan't Open The File");
        return result;
    }

    fseek(filePtr, 0, SEEK_END);
    long fileSize = ftell(filePtr);

    fseek(filePtr, -sizeof(struct inventry), SEEK_END);
    size_t readCount = fread(&inv, sizeof(struct inventry), 1, filePtr);
    if (readCount == 1) {
        result = inv.itemNumber;
    }

    fclose(filePtr);
    return result+1;
}

void addItem(){
    char yesNo;
    FILE*filePntr;

    struct inventry invntry;

    do{
        clear_console();
        filePntr = fopen("inventry.txt","a");
        printf("Add Items To The Inventry\n\n");


        invntry.itemNumber = getLastItemNumber();
        printf("Item Code : %d \n",invntry.itemNumber);
        printf("Enter Item Name : ");
        scanf("%s",&invntry.itemName);
        printf("Enter Item Qty : ");
        scanf("%d",&invntry.itemQty);
        printf("Enter Item Price : ");
        scanf("%lf",&invntry.itemPrice);

        if(filePntr!=NULL){
            printf("\nItem Added Successfully\n");
        }else{
            printf("\nCan't Open The File");
        }

        fwrite(&invntry,sizeof(invntry),1,filePntr);
        fclose(filePntr);

        printf("Do You Want To Add Another New Item ? (y/n) : ");
        scanf("%s",&yesNo);

    }while(yesNo=='y'|| yesNo=='Y');

}

void displayItems(){

    char yesNo;
    clear_console();
    FILE*filePntr;
    struct inventry invntry;

    filePntr = fopen("inventry.txt","r");

    if (filePntr == NULL){
        printf("\nCan't Open The File");
    }else{
        printf("INVENTRY ITEMS\n\n");
    }

    printf("Item Code\t Item\t\t\t Qty\t\t Unit Price\t Total Price\n\n");

    while(fread(&invntry,sizeof(invntry),1,filePntr)){
        double tot = invntry.itemQty*invntry.itemPrice;
        printf("%d\t\t %s\t\t %d\t\t %.2lf\t\t %.2lf\n\n",invntry.itemNumber,invntry.itemName,invntry.itemQty,invntry.itemPrice,tot);
    }

    fclose(filePntr);
}

void updateItem(int itemNumb){
    FILE *filePtr, *tempFilePtr;
    struct inventry inv;

    filePtr = fopen("inventry.txt","r");
    tempFilePtr = fopen("temp_inventry.txt","w");

    printf("Update Item\n");

    if (filePtr == NULL){
         printf("\nCan't Open The File");
    }

    while(fread(&inv,sizeof(inv),1,filePtr)){
        if(inv.itemNumber == itemNumb){
            inv.itemNumber = itemNumb;
            printf("Enter Item Name : ");
            scanf("%s",&inv.itemName);
            printf("Enter Item Qty : ");
            scanf("%d",&inv.itemQty);
            printf("Enter Item Price : ");
            scanf("%lf",&inv.itemPrice);

            fwrite(&inv,sizeof(inv),1,tempFilePtr);
        }else{
            fwrite(&inv,sizeof(inv),1,tempFilePtr);
        }
    }

    fclose(filePtr);
    fclose(tempFilePtr);

    if(remove("inventry.txt") != 0){
        printf("\nError removing the file");
    }
    if(rename("temp_inventry.txt","inventry.txt") != 0){
        printf("\nError renaming the temp file");
    }

}

void searchItem(){
    clear_console();
    FILE *fptr;
    struct inventry inv;
    int itemNumb,found=0;

    fptr = fopen("inventry.txt","r");

    printf("Search & Update Item\n\n\n");
    printf("Enter the Item Code : ");
    scanf("%d",&itemNumb);

    while(fread(&inv,sizeof(inv),1,fptr)){
        if(inv.itemNumber == itemNumb){
            found = 1;
            printf("\n\nItem Code\t Item\t\t Qty\t\t Unit Price\t Total Price\n\n");
            double tot = inv.itemQty*inv.itemPrice;
            printf("%d\t\t %s\t\t %d\t\t %.2lf\t\t %.2lf\n\n",inv.itemNumber,inv.itemName,inv.itemQty,inv.itemPrice,tot);
            break;
        }
    }
    if(found){
        char yesNo;
        printf("Do you Want to Update This Item ? (y/n) : ");
        scanf(" %c",&yesNo);

        if(yesNo=='Y' || yesNo == 'y'){
            fclose(fptr);
            updateItem(itemNumb);
            printf("\n\nItem Updated Successfuly");
        }

    }
    if(!found){
        fclose(fptr);
        printf("\nItem Doesn't Exist");
    }
}

void deleteItem(){
    clear_console();
    FILE *filePtr, *tempFilePtr;
    struct inventry inv;
    int itemNumb,found = 0;

    filePtr = fopen("inventry.txt","r");
    tempFilePtr = fopen("temp_inventry.txt","w");

    printf("Delete Item\n\n\n");
    printf("Enter the Item Code : ");
    scanf("%d",&itemNumb);

    if (filePtr == NULL){
         printf("\nCan't Open The File");
    }

    while(fread(&inv,sizeof(inv),1,filePtr)){
        if(inv.itemNumber == itemNumb){
            found = 1;
            printf("\n\nItem Code\t Item\t\t Qty\t\t Unit Price\t Total Price\n\n");
            double tot = inv.itemQty*inv.itemPrice;
            printf("%d\t\t %s\t\t %d\t\t %.2lf\t\t %.2lf\n\n",inv.itemNumber,inv.itemName,inv.itemQty,inv.itemPrice,tot);
        }else{
            fwrite(&inv,sizeof(inv),1,tempFilePtr);
        }
    }

    fclose(filePtr);
    fclose(tempFilePtr);

    if(found){
        char yesNo;
        printf("Do you Want to Delete This Item ? (y/n) : ");
        scanf("%s",&yesNo);
        switch(yesNo){

            case 'Y' :
            case 'y' :  remove("inventry.txt");
                        rename("temp_inventry.txt","inventry.txt");
                        printf("\n\nItem Deleted Successfuly");
            break;

            case 'N' :
            case 'n' :  remove("temp_inventry.txt");
            break;

            default:
                printf("Invalid choice.\n\n");
        }
    }
    if(!found){
        printf("\nItem Doesn't Exist");
    }
}

int main() {
    int choice;
    while(1) {
        printf("1. Add item\n2. Display items\n3. Search & Update item\n4. Delete item\n5. Exit\n\nEnter your choice: ");
        scanf("%d", &choice);
        switch(choice) {
            case 1:
                addItem();
                clear_console();
                break;
            case 2:
                displayItems();
                if(goBackToMenu()){
                    clear_console();
                    continue;
                }
                break;
            case 3:
                searchItem();
                if(goBackToMenu()){
                    clear_console();
                    continue;
                }
                break;
            case 4:
                deleteItem();
                if(goBackToMenu()){
                    clear_console();
                    continue;
                }
                break;
            case 5:
                exit(0);
            default:
                printf("Invalid choice.\n");
        }
    }
    return 0;
}

