def a_project(projectID):
    supervisor = None

    def the_project(request):
        nonlocal supervisor
        if request == "supervisor": return supervisor
        elif request == "set_supervisor": return set_sv
        elif request == "projectID": return projectID
        else: return "the_project: unknown request " + request

    def set_sv(sv):
        nonlocal supervisor
        supervisor = sv
        return True

    return the_project


def project_supervisor(project):
    sv = project("supervisor")
    if(sv == None):
        return False
    else:
        return supervisorID(sv)

def set_supervisor(project, supervisor):
    return project("set_supervisor")(supervisor)


def projectID(project):
    return project("projectID")


def a_student(studentID):
    student_project = None

    def student(request):
        nonlocal student_project
        if request == 'allocation': return lambda: student_project
        elif request == "studentID": return studentID
        elif request == "choose_project": return set_project
        elif request == "drop_project": return project_false
        else: return "the_project: unknown request " + request

    def set_project(project):
        nonlocal student_project
        if(student_project == None):
            student_project = project
            return True
        return False
    
    def project_false():
        student_project = None
        return True

    return student

def drop_project(student):
    return student("drop_project")

def allocation(student):
    return student('allocation')()


def studentID(student):
    return student("studentID")


def choose_project(student, project):
    return student("choose_project")(project)





def a_supervisor(supervisorID):
    allocated_projects = []
    available_projects = []
    
    supervisor_quota = 6

    def supervisor(request):
        nonlocal supervisor_quota
        if request == "supervisorID": return supervisorID
        elif request == "supervisor_quota": return supervisor_quota
        elif request == "increment":
            if supervisor_quota < 6:
                supervisor_quota += 1
                return True
            else:
                return False
            
        elif request == "decrement":
            if supervisor_quota > 0:
                supervisor_quota -= 1
                return True
            else:
                return False
            
        elif request == 'add_project': return add_project
        elif request == 'allocate': return allocate_project
        elif request == 'deallocate': return deallocate_project
        elif request == 'get_available': return get_available
        elif request == 'get_allocated': return get_allocated_projects
        else: return "the_project: unknown request " + request

    def get_available():
        nonlocal available_projects
        return available_projects

    def get_allocated_projects():
        nonlocal allocated_projects
        return allocated_projects
    
    def add_project(project):
        nonlocal available_projects
        nonlocal allocated_projects
        if(project not in (available_projects and allocated_projects)):
            available_projects.append(projectID(project))
            return True
        else:
            return False

    def allocate_project(project, student):
        nonlocal available_projects
        nonlocal allocated_projects
        
        if((projectID(project)) not in available_projects):
            return False
        else:
            allocated_projects.append((projectID(project), studentID(student)))
            available_projects.remove(projectID(project))
            return True

    def deallocate_project(name):
        nonlocal available_projects
        nonlocal allocated_projects
        
        if name in allocated_projects:
            allocated_projects.remove(name)
            available_projects.append(name[0])
            return True
        else:
            return False

    return supervisor


def supervisorID(supervisor):
    return supervisor("supervisorID")


def supervisor_quota(supervisor):
    return supervisor("supervisor_quota")


def is_underloaded(supervisor):
    if(supervisor_quota(supervisor) > 0):
        return True
    else:
        return False


def add_project(supervisor, project):
    if project_supervisor(project) == False:
        set_supervisor(project, supervisor)
        return supervisor('add_project')(project)
    else:
        return False


def decrement_quota(supervisor):
    return supervisor("decrement")

def increment_quota(supervisor):
    return supervisor("increment")



def allocate_project(supervisor, project, student):
    if project_supervisor(project) == supervisorID(supervisor):
        if projectID(project) in get_available(supervisor):
            if is_underloaded(supervisor) == True:
                choose_project(student, project)
                decrement_quota(supervisor)
                return supervisor('allocate')(project, student)
            else: return False
        else: return False
    else: return False


def deallocate_project(supervisor, project, student):
    if project_supervisor(project) == supervisorID(supervisor):
        for proj in get_allocated(supervisor):
            if proj[0] == projectID(project):
                drop_project(student)
                increment_quota(supervisor)
                return supervisor('deallocate')(proj)
        else: return False
    else: return False




def get_available(supervisor):
    return supervisor('get_available')()


def get_allocated(supervisor):
    return supervisor("get_allocated")()

# TESTS

proj1 = a_project("dlk01")
proj2 = a_project("dlk02")
stud1 = a_student("johnsilver")
stud2 = a_student("robinsoncrusoe")
sup1 = a_supervisor("kazakov")
sup2 = a_supervisor("wood")
#print(allocation(stud1))
#print(allocation(stud2))

add_project(sup1, proj1)
add_project(sup1, proj2)
allocate_project(sup1, proj1, stud1)
#print(allocation(stud1))
allocate_project(sup1, proj2, stud2)
#print(allocation(stud2))
deallocate_project(sup1, proj1, stud1)
deallocate_project(sup1, proj2, stud2)
allocate_project(sup1, proj1, stud2)
allocate_project(sup1, proj2, stud1)
