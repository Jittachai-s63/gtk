// $ sudo apt-get install libgtk-3-dev
/*
gcc -g guitest2.c -o guitest2 -lmosquitto -lpthread -lcurl \
    `pkg-config --cflags gtk+-3.0` \
    `pkg-config --libs gtk+-3.0`
*/ 

////////////////////////////////////////////////////////////////

#include <inttypes.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <glib.h>
#include <gdk/gdk.h>
#include "mosq_sub.c"


GtkApplication *app;
GtkWidget *window, *pic_window ,*tree_window,*mosq_window;
GtkWidget *headerbar , *tree_headerbar;
GtkWidget *head_button1 ;

GtkTreeModel *model = NULL;
GtkWidget *sw;

GMenu *mosqmenu;
GMenu *editmenu;

GtkWidget *gearmenubutton;
GtkWidget *gearicon;
GtkWidget *tree_button, *tree_button2;
GtkWidget *sw, *treeview, *box , *hbox;

GtkWidget *head_label,*sub_label1, *sub_label2, *sub_label3,*sub_label4,*label_topic,*label_sever;
GtkWidget *sub_entry1, *sub_entry2 , *sub_entry3 ;
GtkWidget *sub_button1;

GtkWidget *grid, *sub_grid , *pic_grid;
GtkWidget *pic_box1,*pic_box2,*pic_box3 ;
GtkWidget *img1, *img2, *img3 ,*img_t ;
GtkWidget *main_label1, *main_label2, *main_label3,*main_label4,*main_label5,*main_label6 ;
GtkWidget *main_status ;
GtkWidget *main_button1, *main_button2 ,*main_button3 ;


static gboolean counter_enabled = FALSE;
static gboolean tree_stat = FALSE;
static gboolean win_stat = FALSE;

bool clean_session = true;
struct mosquitto *mosq = NULL;

char *text_entry1,*text_entry2,*text_entry3;
char *sp_cam,temp_str[100],lebel_cam[100] ;
char cam_id1[100], cam_id2[100], cam_id3[100];


void head_button1_callback( GtkWidget* , gpointer ) ;
void tree_button_callback( GtkWidget* , gpointer ) ;
void tree_button2_callback( GtkWidget* , gpointer ) ;
gboolean
foreach_func(GtkTreeModel *, GtkTreePath *, GtkTreeIter *,gpointer );
void main_button1_callback( GtkWidget* , gpointer ) ;
void main_full_pic( GtkWidget* , gpointer ) ;
void main_button2_callback( GtkWidget* , gpointer ) ;
void sub_button1_callback( GtkWidget* , gpointer ) ;
void server_button( GtkWidget* , gpointer ) ;
void topic_button( GtkWidget* , gpointer ) ;

void activate( GtkApplication* , gpointer ) ;
void sub_activate( GtkApplication* , gpointer ) ;
void main_activate( GtkApplication*, gpointer ) ;

static gboolean name_pic( gpointer );

GtkTreeModel *create_model () ;
void add_columns (GtkTreeView *) ;


typedef struct {

    char inDate[100];
    char iD[100];
    char Time[100];

} student;

enum {

  COLUMN_DATE,
  COLUMN_ID,
  COLUMN_TIME,
  NUM_COLUMNS

};
student text_data[1000] ;

const GActionEntry app_actions[] = {
    { "server", server_button },
    { "topic", topic_button }
};


int main( int argc,char **argv ) {

    //to_connect_mqtt( mosq , clean_session);

    //---------------------------------------

    int status;
	app = gtk_application_new( NULL, G_APPLICATION_FLAGS_NONE );
    g_timeout_add_seconds( 1 /*sec*/, G_SOURCE_FUNC(name_pic), NULL );

	g_signal_connect( app, "activate", 
		G_CALLBACK(activate), NULL );

	g_signal_connect( app, "activate", 
		G_CALLBACK(sub_activate), NULL );

    g_action_map_add_action_entries(G_ACTION_MAP (app), app_actions,
                                  G_N_ELEMENTS (app_actions), NULL);

	// start the application main loop (blocking call)
	status = g_application_run( G_APPLICATION(app), argc, argv );

	// decrease the reference count to the object
	g_object_unref( app );

    /*
    FILE *fp;
    fp = fopen("data.txt","w");
    fprintf(fp,"");
    fclose(fp);
    */

	return status ;
}


void sub_button1_callback( GtkWidget *widget, gpointer data ) {

    GtkWidget *dialog;
    win_stat = TRUE ;
    char number[10] = "123456789" ;
    text_entry1 = gtk_entry_get_text (GTK_ENTRY (sub_entry1));
    text_entry2 = gtk_entry_get_text (GTK_ENTRY (sub_entry2));
    text_entry3 = gtk_entry_get_text (GTK_ENTRY (sub_entry3));

    bool t1=false,t2=false,t3=false ;

    int i=0;
    if(strlen(text_entry1)== 1 && strlen(text_entry2)== 1 && strlen(text_entry3)== 1)
    {
        for(i=0;i<9;i++)
        {
            if(text_entry1[0]==number[i])
            {
                t1 = true;
            }
            if(text_entry2[0]==number[i])
            {
                t2 = true;
            }
            if(text_entry3[0]==number[i])
            {
                t3 = true;
            }
        }

        if( t1 && t2 && t3 ){
            sprintf(cam_id1,"cam%s",gtk_entry_get_text (GTK_ENTRY (sub_entry1)));
            sprintf(cam_id2,"cam%s",gtk_entry_get_text (GTK_ENTRY (sub_entry2)));
            sprintf(cam_id3,"cam%s",gtk_entry_get_text (GTK_ENTRY (sub_entry3)));
            
            gtk_widget_destroy(sub_grid);
            main_activate(app,NULL);
            counter_enabled = TRUE ;
        }

        else{
        //gtk_widget_show(sub_label4);
        dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "\n-----Try Again-----\n Please enter 1-9");
        gtk_window_set_title(GTK_WINDOW(dialog), "Error");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        }

    }

    else{
        //gtk_widget_show(sub_label4);
        dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "-----Try Again-----\n Please enter 1-9");
        gtk_window_set_title(GTK_WINDOW(dialog), "Error");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }

}

void main_button2_callback( GtkWidget *widget, gpointer data ) {

	gtk_widget_destroy(grid);
	sub_activate(app,NULL);
    counter_enabled = FALSE ;
    win_stat = FALSE ;
	
}

void main_button1_callback( GtkWidget *widget, gpointer data ) {

	counter_enabled = !counter_enabled ;
    gtk_button_set_label( data,
			counter_enabled ? "Stop" : "Continue" );

}

void head_button1_callback( GtkWidget *widget, gpointer data ) {

    if(win_stat){

        if(tree_stat){
        gtk_widget_destroy(tree_window);
        }

        tree_stat = TRUE;

        tree_window = gtk_application_window_new( app );
        gtk_window_set_title( GTK_WINDOW(tree_window), "List" );
        gtk_window_set_default_size( GTK_WINDOW(tree_window), 320, 480 );
        gtk_window_set_position( GTK_WINDOW(tree_window), GTK_WIN_POS_CENTER );

        tree_headerbar = gtk_header_bar_new ();
        gtk_widget_show(tree_headerbar);
        gtk_window_set_application(GTK_WINDOW(tree_window), GTK_APPLICATION (app));
        gtk_header_bar_set_title(GTK_HEADER_BAR(tree_headerbar), "List ");
        gtk_header_bar_set_subtitle(GTK_HEADER_BAR(tree_headerbar), "Demo Application");
        gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(tree_headerbar), TRUE);
        gtk_window_set_titlebar (GTK_WINDOW(tree_window), tree_headerbar);

        tree_button = gtk_button_new_with_label("Refresh");
        gtk_header_bar_pack_end(GTK_HEADER_BAR(tree_headerbar), tree_button);
        g_signal_connect( tree_button, "clicked", 
            G_CALLBACK(tree_button_callback), NULL );

        box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_container_add(GTK_CONTAINER(tree_window), box);

        tree_button2 = gtk_button_new_with_label("Open");
        gtk_box_pack_end( GTK_BOX(box), tree_button2,FALSE , FALSE , 0 );
        g_signal_connect( tree_button2, "clicked", 
            G_CALLBACK(tree_button2_callback), NULL );

        sw = gtk_scrolled_window_new (NULL, NULL);
        gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (sw),
                                            GTK_SHADOW_ETCHED_IN);
        gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (sw),
                                        GTK_POLICY_NEVER,
                                        GTK_POLICY_AUTOMATIC);
        gtk_box_pack_start (GTK_BOX (box), sw, TRUE, TRUE, 0);

        model = create_model ();

        /* create tree view */
        treeview = gtk_tree_view_new_with_model (model);
        gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview)),

                                GTK_SELECTION_SINGLE);
        g_object_unref (model);
        gtk_container_add (GTK_CONTAINER (sw), treeview);

        /* add columns to the tree view */
        add_columns (GTK_TREE_VIEW (treeview));

        g_signal_connect(treeview, "row-activated", 
            G_CALLBACK(tree_button2_callback) , NULL);

        gtk_widget_show_all(tree_window);
    }

}

void tree_button_callback( GtkWidget *widget, gpointer data ) {

    gtk_widget_destroy(treeview);
    gtk_widget_destroy(sw);

    sw = gtk_scrolled_window_new (NULL, NULL);
        gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (sw),
                                            GTK_SHADOW_ETCHED_IN);
        gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (sw),
                                        GTK_POLICY_NEVER,
                                        GTK_POLICY_AUTOMATIC);
        gtk_box_pack_start (GTK_BOX (box), sw, TRUE, TRUE, 0);

    model = create_model ();

    /* create tree view */
    treeview = gtk_tree_view_new_with_model (model);
    //gtk_tree_view_set_search_column (GTK_TREE_VIEW (treeview),
    //                                COLUMN_DATE);
    gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview)),
                            GTK_SELECTION_SINGLE);
    g_object_unref (model);
    gtk_container_add (GTK_CONTAINER (sw), treeview);
        /* add columns to the tree view */
    add_columns (GTK_TREE_VIEW (treeview));

    g_signal_connect(treeview, "row-activated", 
        G_CALLBACK(tree_button2_callback) , NULL);

    gtk_widget_show_all(tree_window);
}

void tree_button2_callback( GtkWidget *widget, gpointer data ) {

    GtkTreeModel *c_model;
    GtkTreeIter iter;
    GtkTreeSelection *selection;
    GtkWidget *fpic_box ,*f_img ;
    
    gchar *name ;
    gchar *c_time , *c_date ;
    char name_pic[100] ;

    c_model = gtk_tree_view_get_model(treeview);
    
    selection = gtk_tree_view_get_selection (treeview);
    
    if (gtk_tree_selection_get_selected (selection, &c_model, &iter))
    {   
        gtk_tree_model_get(c_model,&iter,
                                COLUMN_DATE, &c_date,
                                COLUMN_ID, &name,
                                COLUMN_TIME, &c_time,
                                -1);

        //g_print("Row : Age %s, Name %s, Result %s\n",name, c_date, c_time);
    }
    
    sprintf(name_pic,"sub_img/%s_%s_%s.jpg",name,c_date,c_time);
    printf("%s\n",name_pic);

    pic_window = gtk_application_window_new( app );
	gtk_window_set_title( GTK_WINDOW(pic_window), name_pic );
	gtk_window_set_default_size( GTK_WINDOW(pic_window), 400, 300 );
	gtk_window_set_position( GTK_WINDOW(pic_window), GTK_POS_RIGHT );
	
    fpic_box = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
    gtk_container_add(GTK_CONTAINER (pic_window), fpic_box);

    f_img = gtk_image_new_from_file(name_pic);
    
    gtk_container_add(GTK_BOX (fpic_box), f_img);

    gtk_widget_show_all(pic_window);
}

void server_button( GtkWidget *widget, gpointer data ) {
    printf("hi\n");
    mosq_window = gtk_application_window_new( app );
	
	gtk_window_set_title( GTK_WINDOW(mosq_window), "Server" );
	gtk_window_set_default_size( GTK_WINDOW(mosq_window), 100, 50 );
    gtk_window_set_resizable (GTK_WINDOW(mosq_window), FALSE);
	gtk_window_set_position( GTK_WINDOW(mosq_window), GTK_WIN_POS_CENTER );

    gtk_widget_show_all(mosq_window);
}

void topic_button( GtkWidget *widget, gpointer data ) {
    printf("hello\n");
}

void activate( GtkApplication *app, gpointer user_data ){

    window = gtk_application_window_new( app );
	
	gtk_window_set_title( GTK_WINDOW(window), "smk" );
	gtk_window_set_default_size( GTK_WINDOW(window), 1750, 800 );
    gtk_window_set_resizable (GTK_WINDOW(window), FALSE);
	gtk_window_set_position( GTK_WINDOW(window), GTK_WIN_POS_CENTER );
	
    
    headerbar = gtk_header_bar_new ();
    gtk_widget_show(headerbar);
    gtk_window_set_application(GTK_WINDOW(window), GTK_APPLICATION (app));
    gtk_header_bar_set_title(GTK_HEADER_BAR(headerbar), "SMK ");
    gtk_header_bar_set_subtitle(GTK_HEADER_BAR(headerbar), "Demo Application");
    gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(headerbar), TRUE);
    gtk_window_set_titlebar (GTK_WINDOW(window), headerbar);

    gearmenubutton = gtk_menu_button_new();
    gearicon = gtk_image_new_from_icon_name("emblem-system-symbolic",
                                          GTK_ICON_SIZE_SMALL_TOOLBAR);
    gtk_button_set_image(GTK_BUTTON(gearmenubutton), gearicon);
    gtk_header_bar_pack_end(GTK_HEADER_BAR(headerbar), gearmenubutton);

    mosqmenu = g_menu_new();
    g_menu_append(mosqmenu, "Server","app.server");
    g_menu_append(mosqmenu, "Topic", "app.topic");
    editmenu = g_menu_new();
    g_menu_append(editmenu, "Coming soon...","app");
    //g_menu_append(editmenu, "_Add", "app.add");
    //g_menu_append(editmenu, "_Delete", "app.delete");
    g_menu_append_section(mosqmenu, NULL, G_MENU_MODEL(editmenu));
    gtk_menu_button_set_menu_model(GTK_MENU_BUTTON(gearmenubutton),
                                    G_MENU_MODEL(mosqmenu));
    //g_object_unref(editmenu);
    g_object_unref(mosqmenu);

    head_button1 = gtk_button_new_with_label("LIST");
    gtk_header_bar_pack_end(GTK_HEADER_BAR(headerbar), head_button1);
    g_signal_connect( head_button1, "clicked", 
		G_CALLBACK(head_button1_callback), NULL );
}

void sub_activate( GtkApplication *app, gpointer user_data ) {

	sub_grid = gtk_grid_new();
    gtk_container_add (GTK_CONTAINER(window), sub_grid);
    gtk_grid_set_column_spacing ( GTK_GRID ( sub_grid ), 9 );
    gtk_grid_set_row_spacing ( GTK_GRID ( sub_grid ), 9 );

    gtk_widget_set_margin_top ( sub_grid, 50 );
    gtk_widget_set_margin_start ( sub_grid, 0 );

/////////////////////////////////////////////////////////////////////////////

    head_label = gtk_label_new("Smoke Detection");
    gtk_widget_override_font(head_label,
                            pango_font_description_from_string("FreeSerif 50"));
    gtk_grid_attach (GTK_GRID (sub_grid), head_label, 0, 0, 1, 1);
    gtk_widget_set_margin_top ( head_label, 0 );
    gtk_widget_set_margin_start ( head_label, 650 );
    gtk_widget_set_margin_end ( head_label, 0 );

//////////////////////////////////////////////////////////////////////////////

    img1 = gtk_image_new_from_file("test_photo.jpg");
    gtk_grid_attach (GTK_GRID (sub_grid), img1, 0, 1, 1, 1);
    gtk_widget_set_size_request(img1,400,300);
    gtk_widget_set_margin_start ( img1, 650 );
    gtk_widget_set_margin_end ( img1, 0 );
    gtk_widget_set_margin_top ( img1, 20 );

    sub_label1 = gtk_label_new("CAM ID:");
    gtk_widget_override_font(sub_label1,
                            pango_font_description_from_string("Monospace 12"));
    gtk_grid_attach (GTK_GRID (sub_grid), sub_label1, 0, 2, 1, 1);
    gtk_widget_set_margin_start ( sub_label1, 470 );
    gtk_widget_set_margin_end ( sub_label1, 0 );
    gtk_widget_set_margin_top ( sub_label1, 20 );

    sub_label2 = gtk_label_new("CAM ID:");
    gtk_widget_override_font(sub_label2,
                            pango_font_description_from_string("Monospace 12"));
    gtk_grid_attach (GTK_GRID (sub_grid), sub_label2, 0, 3, 1, 1);
    gtk_widget_set_margin_start ( sub_label2, 470 );
    gtk_widget_set_margin_end ( sub_label2, 0 );

    sub_label3 = gtk_label_new("CAM ID:");
    gtk_widget_override_font(sub_label3,
                            pango_font_description_from_string("Monospace 12"));
    gtk_grid_attach (GTK_GRID (sub_grid), sub_label3, 0, 4, 1, 1);
    gtk_widget_set_margin_start ( sub_label3, 470 );
    gtk_widget_set_margin_end ( sub_label3, 0 );

    sub_entry1 = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(sub_entry1),"1");
    gtk_widget_set_margin_top ( sub_entry1, 20 );
    gtk_grid_attach (GTK_GRID (sub_grid), sub_entry1, 0, 2, 1, 1);
    gtk_widget_set_margin_start ( sub_entry1, 835);
    gtk_widget_set_margin_end ( sub_entry1, 100 );

    sub_entry2 = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(sub_entry2),"2");
    gtk_grid_attach (GTK_GRID (sub_grid), sub_entry2, 0, 3, 1, 1);
    gtk_widget_set_margin_start ( sub_entry2, 835 );
    gtk_widget_set_margin_end ( sub_entry2, 100 );

    sub_entry3 = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(sub_entry3),"3");
    gtk_grid_attach (GTK_GRID (sub_grid), sub_entry3, 0, 4, 1, 1);
    gtk_widget_set_margin_start ( sub_entry3, 835 );
    gtk_widget_set_margin_end ( sub_entry3, 100 );

    sub_button1 = gtk_button_new_with_label( "Start" );
    g_signal_connect( sub_button1, "clicked",
		G_CALLBACK(sub_button1_callback), sub_button1 );
    gtk_grid_attach (GTK_GRID (sub_grid), sub_button1, 0, 5, 1, 1);
    gtk_widget_override_font(sub_button1,
                            pango_font_description_from_string("FreeSans,Bold 12"));
    gtk_widget_set_margin_start ( sub_button1, 760 );
    gtk_widget_set_margin_end ( sub_button1, 120 );
    gtk_widget_set_margin_top ( sub_button1, 20 );

    label_sever = gtk_label_new("Server : 26.90.194.251 ");
    gtk_widget_override_font(label_sever,
                            pango_font_description_from_string("Monospace 9"));
    gtk_grid_attach (GTK_GRID (sub_grid), label_sever, 0, 6, 1, 1);
    gtk_widget_set_margin_start ( label_sever, 0 );
    gtk_widget_set_margin_end ( label_sever, 870 );
    gtk_widget_set_margin_top ( label_sever, 80 );

    label_topic = gtk_label_new(" Topic  : KJPIE");
    gtk_widget_override_font(label_topic,
                            pango_font_description_from_string("Monospace 9"));
    gtk_grid_attach (GTK_GRID (sub_grid), label_topic, 0, 7, 1, 1);
    gtk_widget_set_margin_start ( label_topic, 0 );
    gtk_widget_set_margin_end ( label_topic, 940 );
    gtk_widget_set_margin_top ( label_topic, 0 );

    main_status = gtk_label_new_with_mnemonic("");
    gtk_grid_attach (GTK_GRID (sub_grid), main_status, 3, 7, 1, 1);
    gtk_widget_set_margin_start ( main_status, 500 );

    if(is_connected){

        gtk_label_set_text(GTK_LABEL(main_status),"• Connect");
        gtk_widget_override_font(main_status,
                            pango_font_description_from_string("Monospace 9"));

    }
    else{

        gtk_label_set_text(GTK_LABEL(main_status),"• Disonnect");
        gtk_widget_override_font(main_status,
                            pango_font_description_from_string("Monospace 10"));

    }

    gtk_widget_show_all(window);

}

void main_activate( GtkApplication *app, gpointer user_data ) {

    gtk_widget_show(head_button1);

	grid = gtk_grid_new();
    gtk_container_add (GTK_CONTAINER(window), grid);
    gtk_grid_set_column_spacing ( GTK_GRID ( grid ), 3 );
    gtk_grid_set_row_spacing ( GTK_GRID ( grid ), 3 );

	pic_box1 = gtk_event_box_new();
    gtk_widget_set_size_request(pic_box1,576,432);

	img1 = gtk_image_new_from_file("cam1_06-10-2021_21-31-23.jpg");
	gtk_container_add(GTK_CONTAINER(pic_box1), img1);
  	gtk_grid_attach (GTK_GRID (grid), pic_box1, 0, 0, 3, 1);
    gtk_widget_set_margin_top ( img1 , 20 );

	pic_box2 = gtk_event_box_new();
    gtk_widget_set_size_request(pic_box2,576,432);

	img2 = gtk_image_new_from_file("cam1_06-10-2021_21-31-23.jpg");
	gtk_container_add(GTK_CONTAINER(pic_box2), img2);
    gtk_grid_attach (GTK_GRID (grid), pic_box2, 3, 0, 3, 1);
    gtk_widget_set_margin_top ( img2 , 20 );

	pic_box3 = gtk_event_box_new();
    gtk_widget_set_size_request(pic_box3,576,432);

	img3 = gtk_image_new_from_file("cam1_06-10-2021_21-31-23.jpg");
	gtk_container_add(GTK_CONTAINER(pic_box3), img3);
    gtk_grid_attach (GTK_GRID (grid), pic_box3, 6, 0, 3, 1);
    gtk_widget_set_margin_top ( img3 , 20 );

	main_label1 = gtk_label_new_with_mnemonic( cam_id1 );
    gtk_widget_override_font(main_label1,
                            pango_font_description_from_string("Tahoma 20"));
    gtk_widget_set_size_request(main_label1,260,40);
    gtk_grid_attach (GTK_GRID (grid), main_label1, 1, 1, 1, 1);
    gtk_widget_set_margin_top ( main_label1 , 20 );

	main_label2 = gtk_label_new_with_mnemonic( cam_id2 );
    gtk_widget_override_font(main_label2,
                            pango_font_description_from_string("Tahoma 20"));
    gtk_widget_set_size_request(main_label2,260,40);
    gtk_grid_attach (GTK_GRID (grid), main_label2, 4, 1, 1, 1);
    gtk_widget_set_margin_top ( main_label2 , 20 );

	main_label3 = gtk_label_new_with_mnemonic( cam_id3 );
    gtk_widget_override_font(main_label3,
                            pango_font_description_from_string("Tahoma 20"));
    gtk_widget_set_size_request(main_label3,260,40);
    gtk_grid_attach (GTK_GRID (grid), main_label3, 7, 1, 1, 1);
    gtk_widget_set_margin_top ( main_label3 , 20 );

	main_label4 = gtk_label_new_with_mnemonic("00-00-00");
    gtk_widget_override_font(main_label4,
                            pango_font_description_from_string("Tahoma 20"));
    gtk_widget_set_size_request(main_label4,260,40);
    gtk_grid_attach (GTK_GRID (grid), main_label4, 1, 2, 1, 1);
    
	main_label5 = gtk_label_new_with_mnemonic("00-00-00");
    gtk_widget_override_font(main_label5,
                            pango_font_description_from_string("Tahoma 20"));
    gtk_widget_set_size_request(main_label5,260,40);
    gtk_grid_attach (GTK_GRID (grid), main_label5, 4, 2, 1, 1);

	main_label6 = gtk_label_new_with_mnemonic("00-00-00");
    gtk_widget_override_font(main_label6,
                            pango_font_description_from_string("Tahoma 20"));
    gtk_widget_set_size_request(main_label6,260,40);
    gtk_grid_attach (GTK_GRID (grid), main_label6, 7, 2, 1, 1);
    
    main_button1 = gtk_button_new_with_label( "Stop" );
    g_signal_connect( main_button1, "clicked",
		G_CALLBACK(main_button1_callback), main_button1 );
    gtk_grid_attach (GTK_GRID (grid), main_button1, 4, 5, 1, 1);

    main_button3 = gtk_button_new_with_label( "Back" );
    g_signal_connect( main_button3, "clicked",
		G_CALLBACK(main_button2_callback), main_button3 );
    gtk_grid_attach (GTK_GRID (grid), main_button3, 5, 5, 1, 1);

    main_status = gtk_label_new_with_mnemonic("");
    gtk_grid_attach (GTK_GRID (grid), main_status, 8, 4, 1, 1);

    if(is_connected){
        gtk_label_set_text(GTK_LABEL(main_status),"• Connect");
    }
    else{
        gtk_label_set_text(GTK_LABEL(main_status),"• Disonnect");
    }

	gtk_widget_show_all(window);

}

static gboolean name_pic( gpointer data ) {

	if(counter_enabled)
	{
		if(strstr(sub_text,cam_id1))
        {   
			strcpy(temp_str,sub_text);
			sp_cam = strtok(temp_str,"_");
			sp_cam = strtok(NULL,"_");
            sprintf(lebel_cam,"%s_%s",sp_cam,strtok(NULL,"_"));

            gtk_widget_override_font(main_label4,
                                pango_font_description_from_string("Tahoma 20"));
            gtk_label_set_text(GTK_LABEL(main_label4),lebel_cam);
			/* cam1_25-09-2021_17-14-49 */

            sprintf(temp_str,"sub_img/%s.jpg",sub_text);

			gtk_image_set_from_file(GTK_IMAGE(img1), temp_str);			

        }
		if(strstr(sub_text,cam_id2))
        {   
			strcpy(temp_str,sub_text);
			sp_cam = strtok(temp_str,"_");
			sp_cam = strtok(NULL,"_");
            sprintf(lebel_cam,"%s_%s",sp_cam,strtok(NULL,"_"));

            gtk_widget_override_font(main_label5,
                                pango_font_description_from_string("Tahoma 20"));
            gtk_label_set_text(GTK_LABEL(main_label5),lebel_cam);
			
            sprintf(temp_str,"sub_img/%s.jpg",sub_text);
			
			gtk_image_set_from_file(GTK_IMAGE(img2), temp_str);
			
        }
		if(strstr(sub_text,cam_id3))
        {   
			strcpy(temp_str,sub_text);
			sp_cam = strtok(temp_str,"_");
			sp_cam = strtok(NULL,"_");
            sprintf(lebel_cam,"%s_%s",sp_cam,strtok(NULL,"_"));

            gtk_widget_override_font(main_label6,
                                pango_font_description_from_string("Tahoma 20"));
            gtk_label_set_text(GTK_LABEL(main_label6),lebel_cam);

            sprintf(temp_str,"sub_img/%s.jpg",sub_text);
			
			gtk_image_set_from_file(GTK_IMAGE(img3), temp_str);
			
        }

	}

	return TRUE; 
}


GtkTreeModel *create_model() {

    gint i = 0;
    GtkListStore *store;
    GtkTreeIter iter;

    /* create list store */
    store = gtk_list_store_new (NUM_COLUMNS,
                                
                                G_TYPE_STRING,   /* iD */
                                G_TYPE_STRING,    /* inDate */
                                G_TYPE_STRING

                                );    

    FILE *fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int run_line ;
    
    run_line=0 ;
    fp = fopen("data.txt","r");
    
    while ((read = getline(&line, &len, fp)) != -1) {

        char cp_text[100] ;
        char *sp_text;
        strcpy(cp_text,line);
        sp_text = strtok(cp_text,"\n");
        strcpy(cp_text,sp_text);
        
        sp_text = strtok(cp_text,"_");
        strcpy(text_data[run_line].iD,sp_text);

        sp_text = strtok(NULL,"_");
        strcpy(text_data[run_line].inDate,sp_text);

        sp_text = strtok(NULL,"_");
        strcpy(text_data[run_line].Time,sp_text);

        run_line++;

    }

    fclose(fp);

    /* add data to the list store */
    for (i = 0; i < run_line; i++)
    {
        gtk_list_store_append (store, &iter);
        gtk_list_store_set (store, &iter,
                            
                            COLUMN_ID, text_data[i].iD,
                            COLUMN_DATE, text_data[i].inDate,
                            COLUMN_TIME, text_data[i].Time,

                            //COLUMN_ID, data[i].iD,
                            //COLUMN_DATE, data[i].inDate,
                            
                            -1);
  }

  return GTK_TREE_MODEL (store);
}

void add_columns (GtkTreeView *treeview) {

    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    GtkTreeModel *model = gtk_tree_view_get_model (treeview);

    /* column for iD */
    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes ("Cam ID",
            renderer,
            "text",
            COLUMN_ID,
            NULL);
    gtk_tree_view_column_set_sort_column_id (column, COLUMN_ID);
    gtk_tree_view_append_column (treeview, column);

    /* column for inscription date */
    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes ("Date",
            renderer,
            "text",
            COLUMN_DATE,
            NULL);
    gtk_tree_view_column_set_sort_column_id (column, COLUMN_DATE);
    gtk_tree_view_append_column (treeview, column);

    /* column for inscription date */
    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes ("Time",
            renderer,
            "text",
            COLUMN_TIME,
            NULL);
            
    gtk_tree_view_column_set_sort_column_id (column, COLUMN_TIME);
    gtk_tree_view_append_column (treeview, column);
  
}



